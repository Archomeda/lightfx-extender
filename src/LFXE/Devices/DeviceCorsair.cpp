#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceCorsair.h"

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>

// 3rd party includes
#include "CUESDK.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + message)

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;


CORSAIRSETLEDSCOLORS Corsair_SetLedsColors;
CORSAIRSETLEDSCOLORSASYNC Corsair_SetLedsColorsAsync;
CORSAIRGETDEVICECOUNT Corsair_GetDeviceCount;
CORSAIRGETDEVICEINFO Corsair_GetDeviceInfo;
CORSAIRGETLEDPOSITIONS Corsair_GetLedPositions;
CORSAIRGETLEDIDFORKEYNAME Corsair_GetLedIdForKeyName;
CORSAIRREQUESTCONTROL Corsair_RequestControl;
CORSAIRPERFORMPROTOCOLHANDSHAKE Corsair_PerformProtocolHandshake;
CORSAIRGETLASTERROR Corsair_GetLastError;

HINSTANCE hInstanceCorsair = NULL;

namespace lightfx {
    namespace devices {

        LFXE_API void DeviceCorsair::SetRange(const int outMin, const int outMax, const int inMin, const int inMax) {
            this->rangeOutMin = outMin;
            this->rangeOutMax = outMax;
            this->rangeInMin = inMin;
            this->rangeInMax = inMax;
        }

        LFXE_API bool DeviceCorsair::Initialize() {
            if (!this->IsInitialized()) {
                if (Device::Initialize()) {
                    // Load the library
                    if (!hInstanceCorsair) {
                        hInstanceCorsair = LoadLibraryA(CORSAIR_DLL_NAME);
                    }

                    if (!hInstanceCorsair) {
                        LOG(LogLevel::Error, L"Failed to initialize the CUESDK library");
                        this->SetInitialized(false);
                        return false;
                    }

                    // Load the function addresses
                    Corsair_SetLedsColors = (CORSAIRSETLEDSCOLORS)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_SETLEDSCOLORS);
                    Corsair_SetLedsColorsAsync = (CORSAIRSETLEDSCOLORSASYNC)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_SETLEDSCOLORSASYNC);
                    Corsair_GetDeviceCount = (CORSAIRGETDEVICECOUNT)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_GETDEVICECOUNT);
                    Corsair_GetDeviceInfo = (CORSAIRGETDEVICEINFO)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_GETDEVICEINFO);
                    Corsair_GetLedPositions = (CORSAIRGETLEDPOSITIONS)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_GETLEDPOSITIONS);
                    Corsair_GetLedIdForKeyName = (CORSAIRGETLEDIDFORKEYNAME)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_GETLEDIDFORKEYNAME);
                    Corsair_RequestControl = (CORSAIRREQUESTCONTROL)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_REQUESTCONTROL);
                    Corsair_PerformProtocolHandshake = (CORSAIRPERFORMPROTOCOLHANDSHAKE)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_PERFORMPROTOCOLHANDSHAKE);
                    Corsair_GetLastError = (CORSAIRGETLASTERROR)GetProcAddress(hInstanceCorsair, CORSAIR_DLL_GETLASTERROR);

                    // Just do an initial pass to set how many LEDs there are available
                    this->SetNumberOfLights(1);
                    this->SetLightData(0, LightData());

                    this->Reset();
                }
                else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceCorsair::Release() {
            if (this->IsInitialized()) {
                if (Device::Release()) {
                    if (hInstanceCorsair) {
                        bool result = FreeLibrary(hInstanceCorsair) == TRUE;
                        hInstanceCorsair = NULL;
                        return result;
                    }
                } else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceCorsair::Enable() {
            if (!this->IsEnabled()) {
                if (Device::Enable()) {
                    Corsair_PerformProtocolHandshake();
                    if (const auto error = Corsair_GetLastError()) {
                        const char* str_error = toString(error);
                        wstring wstr(str_error, str_error + strlen(str_error));
                        LOG(LogLevel::Error, L"Handshake with Corsair failed: " + wstr);
                        this->SetEnabled(false);
                        return false;
                    }
                    else {
                        this->Reset();
                        this->ledPositions = Corsair_GetLedPositions();
                    }
                }
                else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceCorsair::PushColorToDevice(const vector<LightColor>& colors) {
            double red = colors[0].red;
            double green = colors[0].green;
            double blue = colors[0].blue;
            double alpha = colors[0].brightness;

            double divider = (this->rangeOutMax - this->rangeOutMin) / ((this->rangeInMax - this->rangeInMin) / 100.0) / 100.0;

            int updated_red = (int)(red * (alpha / 255.0));
            int updated_green = (int)(green * (alpha / 255.0));
            int updated_blue = (int)(blue * (alpha / 255.0));

            std::vector<CorsairLedColor> vec;

            if (this->ledPositions) {
                for (auto i = 0; i < this->ledPositions->numberOfLed; i++) {
                    auto ledId = this->ledPositions->pLedPosition[i].ledId;
                    vec.push_back(CorsairLedColor{ ledId, updated_red, updated_green, updated_blue });
                }
            }

            LOG(LogLevel::Debug, L"Update color to (" + to_wstring(updated_red) + L"," + to_wstring(updated_green) + L"," + to_wstring(updated_blue) + L")");

            return Corsair_SetLedsColorsAsync(static_cast<unsigned int>(vec.size()), vec.data(), nullptr, nullptr);
        }


        const char* DeviceCorsair::toString(CorsairError error)
        {
            switch (error) {
            case CE_Success:
                return "CE_Success";
            case CE_ServerNotFound:
                return "CE_ServerNotFound";
            case CE_NoControl:
                return "CE_NoControl";
            case CE_ProtocolHandshakeMissing:
                return "CE_ProtocolHandshakeMissing";
            case CE_IncompatibleProtocol:
                return "CE_IncompatibleProtocol";
            case CE_InvalidArguments:
                return "CE_InvalidArguments";
            default:
                return "unknown error";
            }
        }
    }
}

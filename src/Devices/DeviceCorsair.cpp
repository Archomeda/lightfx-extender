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


using namespace std;
using namespace lightfx::devices::proxies;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
    namespace devices {

        LFXE_API bool DeviceCorsair::Initialize() {
            if (!this->IsInitialized()) {
                if (Device::Initialize()) {
                    // Load the library first
                    this->library = unique_ptr<CUESDKProxy>(new CUESDKProxy);
                    if (!this->library->Load()) {
                        LOG_ERROR(L"Failed to access the CUESDK library");
                        this->SetInitialized(false);
                        return false;
                    }

                    // Just do an initial pass to set how many LEDs there are available
                    this->SetNumberOfLights(1);
                    this->SetLightData(0, LightData());

                    this->Reset();
                } else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceCorsair::Release() {
            if (this->IsInitialized()) {
                if (Device::Release()) {
                    bool result = this->library->Unload();
                    this->SetInitialized(!result);
                    return result;
                } else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceCorsair::Enable() {
            if (!this->IsEnabled()) {
                if (Device::Enable()) {
                    this->library->CorsairPerformProtocolHandshake();
                    if (const auto error = this->library->CorsairGetLastError()) {
                        LOG_ERROR(L"Handshake with Corsair failed: " + this->library->CorsairErrorToString(error));
                    } else {
                        this->SetEnabled(false);
                        return false;
                        this->Reset();
                        this->ledPositions = this->library->CorsairGetLedPositions();
                    }
                } else {
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

            LOG_DEBUG(L"Update color to (" + to_wstring(updated_red) + L"," + to_wstring(updated_green) + L"," + to_wstring(updated_blue) + L")");

            return this->library->CorsairSetLedsColorsAsync(static_cast<unsigned int>(vec.size()), vec.data(), nullptr, nullptr);
        }
    }
}

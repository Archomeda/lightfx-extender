#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceLightFX.h"

// Standard includes
#include <limits>

// Project includes
#include "LightFX2.h"
#include "../LightFXExtender.h"
#include "../Managers/LogManager.h"
#include "../Utils/String.h"


#define LOG(logLevel, message) if (this->GetManager() != nullptr) { LOG_(this->GetManager()->GetLightFXExtender()->GetLogManager(), logLevel, wstring(L"LightFX Device ") + this->GetDeviceName() + L" - " + message) }

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
    namespace devices {

        LFXE_API bool DeviceLightFX::Initialize() {
            if (!this->IsInitialized()) {
                if (Device::Initialize()) {
                    // Just do an initial pass to set our configuration
                    LFX_RESULT result;

                    // Set up device name and type
                    char* devDesc = new char[LFX_MAX_STRING_SIZE];
                    unsigned char devType = 0;
                    result = LightFX_GetDeviceDescription(this->GetDeviceIndex(), devDesc, LFX_MAX_STRING_SIZE, &devType);
                    if (result != LFX_SUCCESS) {
                        LFX_SAFE_DELETE_ARRAY(devDesc);
                        return false;
                    }
                    this->SetDeviceName(string_to_wstring(devDesc));
                    LFX_SAFE_DELETE_ARRAY(devDesc);
                    this->SetDeviceType((DeviceType)devType);

                    // Set up number of lights
                    unsigned int numLights = 0;
                    result = LightFX_GetNumLights(this->GetDeviceIndex(), &numLights);
                    if (result != LFX_SUCCESS) {
                        return false;
                    }
                    this->SetNumberOfLights(numLights);

                    // Set up lights
                    for (unsigned int i = 0; i < numLights; ++i) {
                        LightData lightData;

                        char* lightDesc = new char[LFX_MAX_STRING_SIZE];
                        result = LightFX_GetLightDescription(this->GetDeviceIndex(), i, lightDesc, LFX_MAX_STRING_SIZE);
                        if (result != LFX_SUCCESS) {
                            LFX_SAFE_DELETE_ARRAY(lightDesc);
                            return false;
                        }
                        lightData.Name = string_to_wstring(lightDesc);

                        LFX_POSITION lightLoc;
                        result = LightFX_GetLightLocation(this->GetDeviceIndex(), i, &lightLoc);
                        if (result != LFX_SUCCESS) {
                            return false;
                        }
                        lightData.Position = { lightLoc.x, lightLoc.y, lightLoc.z };

                        this->SetLightData(i, lightData);
                    }

                    return true;
                }

            }
            return false;
        }

        LFXE_API const unsigned int DeviceLightFX::GetDeviceIndex() {
            return this->deviceIndex;
        }

        LFXE_API void DeviceLightFX::SetDeviceIndex(const unsigned int deviceIndex) {
            this->deviceIndex = deviceIndex;
        }

        LFXE_API const wstring DeviceLightFX::GetDeviceName() {
            return this->deviceName;
        }

        LFXE_API void DeviceLightFX::SetDeviceName(const wstring& deviceName) {
            this->deviceName = deviceName;
        }

        LFXE_API const DeviceType DeviceLightFX::GetDeviceType() {
            return this->deviceType;
        }

        LFXE_API void DeviceLightFX::SetDeviceType(const DeviceType deviceType) {
            this->deviceType = deviceType;
        }


        LFXE_API bool DeviceLightFX::PushColorToDevice(const vector<LightColor>& colors) {
            if (colors.size() > numeric_limits<unsigned int>::max()) {
                return false;
            }
            unsigned int numLights = static_cast<unsigned int>(colors.size());

            LFX_RESULT result;
            for (unsigned int i = 0; i < numLights; ++i) {
                LFX_COLOR color = { colors[i].red, colors[i].green, colors[i].blue, colors[i].brightness };
                LOG(LogLevel::Debug, L"Update color of light " + this->GetLightData(i).Name + L" (" + to_wstring(i) + L") to (" + to_wstring(colors[i].red) + L"," + to_wstring(colors[i].green) + L"," + to_wstring(colors[i].blue) + L")");
                result = LightFX_SetLightColor(this->GetDeviceIndex(), i, &color);
                if (result != LFX_SUCCESS) {
                    return false;
                }
            }

            result = LightFX_Update();
            return result == LFX_SUCCESS;
        }

    }
}
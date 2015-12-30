#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceLightFX.h"

// Standard includes
#include <limits>

// Project includes
#include "../LightFXExtender.h"
#include "../Config/MainConfigFile.h"
#include "../Managers/ConfigManager.h"
#include "../Utils/Log.h"
#include "../Utils/String.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"LightFX Device ") + this->GetDeviceName() + L" - " + message)

using namespace std;
using namespace lightfx::devices::proxies;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
    namespace devices {

        LFXE_API bool DeviceLightFX::Initialize() {
            if (!this->IsInitialized()) {
                if (Device::Initialize()) {
                    // Load the library first
                    auto config = this->GetManager()->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
                    this->library = unique_ptr<LightFX2Proxy>(new LightFX2Proxy(config->AlienwareDllName, config->AlienwareBackupDllName));
                    if (!this->library->Load()) {
                        LOG(LogLevel::Error, L"Failed to access the Alienware LightFX library");
                        return false;
                    }

                    // Just do an initial pass to set our configuration
                    LFX_RESULT result;

                    // Set up device name and type
                    char* devDesc = new char[LFX_MAX_STRING_SIZE];
                    unsigned char devType = 0;
                    result = this->library->LFX_GetDeviceDescription(this->GetDeviceIndex(), devDesc, LFX_MAX_STRING_SIZE, &devType);
                    if (result != LFX_SUCCESS) {
                        LOG(LogLevel::Error, L"Couldn't get device description from " + to_wstring(this->GetDeviceIndex()) + L": " + this->library->LfxResultToString(result));
                        LFX_SAFE_DELETE_ARRAY(devDesc);
                        return false;
                    }
                    this->SetDeviceName(string_to_wstring(devDesc));
                    LFX_SAFE_DELETE_ARRAY(devDesc);
                    this->SetDeviceType((DeviceType)devType);

                    // Set up number of lights
                    unsigned int numLights = 0;
                    result = this->library->LFX_GetNumLights(this->GetDeviceIndex(), &numLights);
                    if (result != LFX_SUCCESS) {
                        LOG(LogLevel::Error, L"Couldn't get the number of lights from " + to_wstring(this->GetDeviceIndex()) + L": " + this->library->LfxResultToString(result));
                        return false;
                    }
                    this->SetNumberOfLights(numLights);
                    LOG(LogLevel::Debug, to_wstring(numLights) + L" lights available");

                    // Set up lights
                    for (unsigned int i = 0; i < numLights; ++i) {
                        LightData lightData;

                        // Description
                        char* lightDesc = new char[LFX_MAX_STRING_SIZE];
                        result = this->library->LFX_GetLightDescription(this->GetDeviceIndex(), i, lightDesc, LFX_MAX_STRING_SIZE);
                        if (result == LFX_SUCCESS) {
                            lightData.Name = string_to_wstring(lightDesc);
                        } else {
                            LOG(LogLevel::Warning, L"Couldn't get light description " + to_wstring(i) + L" from " + to_wstring(this->GetDeviceIndex()) + L": " + this->library->LfxResultToString(result) + L" (using default)");
                        }
                        LFX_SAFE_DELETE_ARRAY(lightDesc);
                        if (lightData.Name == L"") {
                            lightData.Name = to_wstring(i);
                        }
                        LOG(LogLevel::Debug, L"Light " + to_wstring(i) + L" name: " + lightData.Name);

                        // Location
                        LFX_POSITION lightLoc;
                        result = this->library->LFX_GetLightLocation(this->GetDeviceIndex(), i, &lightLoc);
                        if (result == LFX_SUCCESS) {
                            lightData.Position = { lightLoc.x, lightLoc.y, lightLoc.z };
                        } else {
                            LOG(LogLevel::Warning, L"Couldn't get light location " + to_wstring(i) + L" from " + to_wstring(this->GetDeviceIndex()) + L": " + this->library->LfxResultToString(result) + L" (using default)");
                            lightData.Position = { 0, 0, 0 };
                        }
                        LOG(LogLevel::Debug, L"Light " + to_wstring(i) + L" pos: (" + to_wstring(lightData.Position.x) + L", " + to_wstring(lightData.Position.y) + L", " + to_wstring(lightData.Position.z) + L")");

                        this->SetLightData(i, lightData);
                    }
                } else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceLightFX::Release() {
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
                result = this->library->LFX_SetLightColor(this->GetDeviceIndex(), i, &color);
                if (result != LFX_SUCCESS) {
                    LOG(LogLevel::Warning, L"SetLightColor() failed: " + this->library->LfxResultToString(result));
                    return false;
                }
            }

            result = this->library->LFX_Update();
            if (result != LFX_SUCCESS) {
                LOG(LogLevel::Warning, L"Update() failed: " + this->library->LfxResultToString(result));
            }
            return result == LFX_SUCCESS;
        }
    }
}

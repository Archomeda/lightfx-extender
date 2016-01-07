#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "VendorAlienware.h"

// Project includes
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::timelines;
using namespace lightfx::utils;
using namespace lightfx::vendors::libraries;

namespace lightfx {
    namespace vendors {

        LFXE_API void VendorAlienware::InitializeDevices() {
            if (this->IsLibraryAvailable()) {
                // Since LFXE is built upon LightFX, we can easily add multiple ALienware devices here
                // But we do have to initialize the library if we haven't already, otherwise we can't query the devices
                bool wasInitialized = this->library->IsLibraryInitialized();
                if (!wasInitialized) {
                    if (!this->InitializeLibrary()) {
                        LOG_WARNING(L"Could not initialize Alienware library");
                    }
                }

                AlienwareResult result;
                unsigned int numDevices = 0;
                result = this->library->Aw_GetNumDevices(&numDevices);
                if (result == AlienwareResult::AlienwareSuccess) {
                    for (unsigned int i = 0; i < numDevices; ++i) {
                        auto device = make_shared<VendorDevice>(shared_from_this());
                        device->SetHardwareId(i);

                        wstring deviceName = L"";
                        AlienwareDeviceType deviceType = AlienwareDeviceType::AlienwareDeviceTypeUnknown;
                        result = this->library->Aw_GetDeviceDescription(i, &deviceName, &deviceType);
                        if (result == AlienwareResult::AlienwareSuccess) {
                            device->SetDeviceName(deviceName);
                            device->SetDeviceType(this->AlienwareDeviceTypeToVendorDeviceType(deviceType));
                        } else {
                            LOG_WARNING(L"Could not get name of device " + to_wstring(i) + L": " + this->library->AlienwareResultToString(result));
                        }

                        unsigned int numLights = 0;
                        result = this->library->Aw_GetNumLights(i, &numLights);
                        if (result == AlienwareResult::AlienwareSuccess) {
                            device->SetNumberOfLights(static_cast<size_t>(numLights));
                            for (unsigned int j = 0; j < numLights; ++j) {
                                VendorDeviceLightData lightData = {};
                                
                                wstring lightName = L"";
                                result = this->library->Aw_GetLightDescription(i, j, &lightName);
                                if (result == AlienwareResult::AlienwareSuccess) {
                                    lightData.name = lightName;
                                } else {
                                    LOG_WARNING(L"Could not get name of light " + to_wstring(j) + L" of device " + to_wstring(i) + L": " + this->library->AlienwareResultToString(result));
                                }

                                LFX_POSITION lightPosition = {};
                                result = this->library->Aw_GetLightLocation(i, j, &lightPosition);
                                if (result == AlienwareResult::AlienwareSuccess) {
                                    lightData.position = this->AlienwareLightPositionToVendorDeviceLightPosition(lightPosition);
                                } else {
                                    LOG_WARNING(L"Could not get position of light " + to_wstring(j) + L" of device " + to_wstring(i) + L": " + this->library->AlienwareResultToString(result));
                                }

                                device->SetLightData(static_cast<size_t>(j), lightData);
                            }
                        } else {
                            LOG_WARNING(L"Could not get lights of device " + to_wstring(i) + L": " + this->library->AlienwareResultToString(result));
                        }

                        this->AddDevice(device->GetHardwareId(), device);
                        LOG_DEBUG(L"Initialized Alienware " + device->GetDeviceName() + L" device");
                    }
                } else {
                    LOG_WARNING(L"Could not number of devices: " + this->library->AlienwareResultToString(result));
                }

                //TODO: Periodically check for changes (e.g. when a device gets connected or disconnected)

                if (wasInitialized) {
                    this->ReleaseLibrary();
                    return;
                }
            }
        }

        LFXE_API bool VendorAlienware::EnableDevice(const unsigned int id) {
            // Alienware doesn't support enabling/disabling devices natively, so we have to emulate it
            if (this->library->InitializeLibrary()) {
                if (!this->IsDeviceEnabled(id)) {
                    this->enabledDevices[id] = true;
                    //TODO: Implement a save state per device to which we can reset to
                }
                return true;
            }
            return false;
        }

        LFXE_API bool VendorAlienware::DisableDevice(const unsigned int id) {
            // Alienware doesn't support enabling/disabling devices natively, so we have to emulate it
            if (this->IsDeviceEnabled(id)) {
                this->enabledDevices[id] = false;
                //TODO: Implement a save state per device to which we can reset to

                bool allDisabled = true;
                for (auto device : this->enabledDevices) {
                    if (device.second) {
                        allDisabled = false;
                        break;
                    }
                }
                if (allDisabled) {
                    // All devices are disabled, release library
                    this->library->ReleaseLibrary();
                }
                return true;
            }
            return false;
        }

        LFXE_API bool VendorAlienware::ResetDevice(const unsigned int id) {
            // Alienware doesn't support resetting devices individually, so we have to emulate it
            if (this->IsDeviceEnabled(id)) {
                //TODO: Implement a save state per device to which we can reset to
            }
            return false;
        }

        LFXE_API bool VendorAlienware::PushColorsToDevice(const unsigned int id, const vector<LightColor>& colors) {
            if (this->IsDeviceEnabled(id) && colors.size() > 0) {
                if (colors.size() > numeric_limits<unsigned int>::max()) {
                    return false;
                }

                bool success = true;
                for (unsigned int i = 0; i < colors.size(); ++i) {
                    LFX_COLOR color = this->TimelineLightColorToAlienwareLightColor(colors[i]);
                    if (this->library->Aw_SetLightColor(id, i, color) != AlienwareResult::AlienwareSuccess) {
                        success = false;
                        break;
                    }
                }
                if (success) {
                    return this->library->Aw_Update() == AlienwareResult::AlienwareSuccess;
                }
            }
            return false;
        }

        LFXE_API VendorDeviceType VendorAlienware::AlienwareDeviceTypeToVendorDeviceType(const AlienwareDeviceType deviceType) const {
            switch (deviceType) {
            case AlienwareDeviceType::AlienwareDeviceTypeNotebook:
                return VendorDeviceType::DeviceNotebook;
            case AlienwareDeviceType::AlienwareDeviceTypeDesktop:
                return VendorDeviceType::DeviceDesktop;
            case AlienwareDeviceType::AlienwareDeviceTypeServer:
                return VendorDeviceType::DeviceServer;
            case AlienwareDeviceType::AlienwareDeviceTypeDisplay:
                return VendorDeviceType::DeviceDisplay;
            case AlienwareDeviceType::AlienwareDeviceTypeMouse:
                return VendorDeviceType::DeviceMouse;
            case AlienwareDeviceType::AlienwareDeviceTypeKeyboard:
                return VendorDeviceType::DeviceKeyboard;
            case AlienwareDeviceType::AlienwareDeviceTypeGamepad:
                return VendorDeviceType::DeviceGamepad;
            case AlienwareDeviceType::AlienwareDeviceTypeSpeaker:
                return VendorDeviceType::DeviceSpeaker;
            case AlienwareDeviceType::AlienwareDeviceTypeUnknown:
            default:
                return VendorDeviceType::DeviceUnknown;
            }
        }

        LFXE_API VendorDeviceLightPosition VendorAlienware::AlienwareLightPositionToVendorDeviceLightPosition(const LFX_POSITION& lightPosition) const {
            VendorDeviceLightPosition newPosition = {};
            newPosition.x = lightPosition.x;
            newPosition.y = lightPosition.y;
            newPosition.z = lightPosition.z;
            return newPosition;
        }

        LFXE_API LFX_COLOR VendorAlienware::TimelineLightColorToAlienwareLightColor(const LightColor& lightColor) const {
            LFX_COLOR newColor = {};
            newColor.red = lightColor.red;
            newColor.green = lightColor.green;
            newColor.blue = lightColor.blue;
            newColor.brightness = lightColor.brightness;
            return newColor;
        }

    }
}

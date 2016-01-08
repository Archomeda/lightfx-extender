#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "VendorCorsair.h"

// Project includes
#include "../Utils/Log.h"
#include "../Utils/String.h"


using namespace std;
using namespace lightfx::timelines;
using namespace lightfx::utils;
using namespace lightfx::vendors::libraries;

namespace lightfx {
    namespace vendors {

        LFXE_API void VendorCorsair::InitializeDevices() {
            if (this->IsLibraryAvailable()) {
                // Corsair SDK supports multiple devices natively
                // But we do have to initialize the library if we haven't already, otherwise we can't query the devices
                bool wasInitialized = this->library->IsLibraryInitialized();
                if (!wasInitialized) {
                    if (!this->InitializeLibrary()) {
                        LOG_WARNING(L"Could not initialize Corsair library");
                    }
                }

                int numDevices = 0;
                numDevices = this->library->Cs_GetDeviceCount();
                if (numDevices >= 0) {
                    for (int i = 0; i < numDevices; ++i) {
                        auto device = make_shared<VendorDevice>(shared_from_this());
                        device->SetHardwareId(i);

                        CorsairDeviceInfo* deviceInfo = this->library->Cs_GetDeviceInfo(i);
                        if (deviceInfo != NULL) {
                            device->SetDeviceType(this->CorsairDeviceTypeToVendorDeviceType(deviceInfo->type));
                        } else {
                            CorsairError error = this->library->Cs_GetLastError();
                            LOG_WARNING(L"Could not get info of device " + to_wstring(i) + L": " + this->library->CorsairErrorToString(error));
                        }

                        VendorDeviceLightData lightData = {};
                        switch (device->GetDeviceType()) {
                        case VendorDeviceType::DeviceKeyboard:
                            device->SetDeviceName(L"Kayboards");
                            device->SetNumberOfLights(148); // For now there are 148 available keyboard LEDs in CorsairLedIdEnum.h
                            for (int i = 1; i <= 147; ++i) {
                                // 1 - 147
                                lightData = {};
                                lightData.name = to_wstring(i);
                                device->SetLightData(i - 1, lightData);
                            }
                            // 154
                            lightData = {};
                            lightData.name = to_wstring(154);
                            device->SetLightData(147, lightData);
                            break;

                        case VendorDeviceType::DeviceMouse:
                            device->SetDeviceName(L"Mice");
                            device->SetNumberOfLights(4); // For now there are 4 available mouse LEDs in CorsairLedIdEnum.h
                            for (int i = 148; i <= 151; ++i) {
                                // 148 - 151
                                lightData = {};
                                lightData.name = to_wstring(i);
                                device->SetLightData(i - 148, lightData);
                            }
                            break;

                        case VendorDeviceType::DeviceHeadset:
                            device->SetDeviceName(L"Headsets");
                            device->SetNumberOfLights(2); // For now there are 2 available headset LEDs in CorsairLedIdEnum.h
                            for (int i = 152; i <= 153; ++i) {
                                // 152 - 153
                                lightData = {};
                                lightData.name = to_wstring(i);
                                device->SetLightData(i - 152, lightData);
                            }
                            break;
                        }

                        this->AddDevice(device->GetHardwareId(), device);
                        LOG_DEBUG(L"Initialized Corsair " + device->GetDeviceName() + L" device");
                    }
                } else {
                    CorsairError error = this->library->Cs_GetLastError();
                    LOG_WARNING(L"Could not get the number of devices: " + this->library->CorsairErrorToString(error));
                }

                //TODO: Check if periodically checking for changes is needed (e.g. when a device gets connected or disconnected)

                if (wasInitialized) {
                    this->ReleaseLibrary();
                    return;
                }
            }
        }

        LFXE_API bool VendorCorsair::EnableDevice(const unsigned int id) {
            // Corsair doesn't support enabling/disabling devices natively, so we have to emulate it
            if (this->library->InitializeLibrary()) {
                if (!this->IsDeviceEnabled(id)) {
                    this->enabledDevices[id] = true;
                    //TODO: Implement a save state per device to which we can reset to
                }
                return true;
            }
            return false;
        }

        LFXE_API bool VendorCorsair::DisableDevice(const unsigned int id) {
            // Corsair doesn't support enabling/disabling devices natively, so we have to emulate it
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

        LFXE_API bool VendorCorsair::ResetDevice(const unsigned int id) {
            // Corsair doesn't support resetting devices at all, so we have to emulate it
            if (this->IsDeviceEnabled(id)) {
                //TODO: Implement a save state per device to which we can reset to
            }
            return false;
        }

        LFXE_API bool VendorCorsair::PushColorsToDevice(const unsigned int id, const vector<LightColor>& colors) {
            if (this->IsDeviceEnabled(id) && colors.size() > 0) {
                if (colors.size() > numeric_limits<unsigned int>::max()) {
                    return false;
                }

                // Corsair doesn't support setting colors to a device directly, it bypasses that by setting the lights through an unique enum
                vector<CorsairLedColor> newColors(colors.size());
                for (unsigned int i = 0; i < colors.size(); ++i) {
                    // Get the LED id
                    VendorDeviceLightData lightData = this->GetDevice(id)->GetLightData(i);
                    CorsairLedId ledId = CorsairLedId::CLI_Invalid;
                    try {
                        ledId = static_cast<CorsairLedId>(stoi(lightData.name));
                    } catch (...) {
                        LOG_WARNING(L"Could not get the LED id of light " + to_wstring(i) + L" of " + this->GetDevice(i)->GetDeviceName());
                    }
                    if (ledId != CorsairLedId::CLI_Invalid) {
                        double scale = colors[i].brightness / 255.0;
                        CorsairLedColor newColor = {};
                        newColor.ledId = ledId;
                        newColor.r = static_cast<int>(colors[i].red * scale);
                        newColor.g = static_cast<int>(colors[i].green * scale);
                        newColor.b = static_cast<int>(colors[i].blue * scale);
                        newColors.push_back(newColor);
                    }
                }
                return this->library->Cs_SetLedsColorsAsync(static_cast<int>(newColors.size()), newColors.data(), NULL, NULL);
            }
            return false;
        }

        LFXE_API VendorDeviceType VendorCorsair::CorsairDeviceTypeToVendorDeviceType(const CorsairDeviceType deviceType) const {
            switch (deviceType) {
            case CorsairDeviceType::CDT_Mouse:
                return VendorDeviceType::DeviceMouse;
            case CorsairDeviceType::CDT_Keyboard:
                return VendorDeviceType::DeviceKeyboard;
            case CorsairDeviceType::CDT_Headset:
                return VendorDeviceType::DeviceHeadset;
            case CorsairDeviceType::CDT_Unknown:
            default:
                return VendorDeviceType::DeviceUnknown;
            }
        }

    }
}

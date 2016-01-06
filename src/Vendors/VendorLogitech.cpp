#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "VendorLogitech.h"

// Project includes
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
    namespace vendors {

        LFXE_API void VendorLogitech::InitializeDevices() {
            if (this->IsLibraryAvailable()) {
                // Logitech SDK supports 3 different device types, so we create 3 separate devices
                auto deviceMonochrome = make_shared<VendorDevice>(shared_from_this());
                deviceMonochrome->SetHardwareId(LOGI_DEVICETYPE_MONOCHROME);
                deviceMonochrome->SetDeviceName(L"Monochrome devices");
                deviceMonochrome->SetDeviceType(VendorDeviceType::DeviceKeyboard); // Since we don't know what kind of monochrome device this is, we simulate the keyboard
                deviceMonochrome->SetNumberOfLights(1);
                this->AddDevice(deviceMonochrome->GetHardwareId(), deviceMonochrome);
                LOG_DEBUG(L"Initialized Logitech monochrome device");

                auto deviceRgb = make_shared<VendorDevice>(shared_from_this());
                deviceRgb->SetHardwareId(LOGI_DEVICETYPE_RGB);
                deviceRgb->SetDeviceName(L"Full RGB devices");
                deviceRgb->SetDeviceType(VendorDeviceType::DeviceKeyboard); // Since we don't know what kind of RGB device this is, we simulate the keyboard
                deviceRgb->SetNumberOfLights(1);
                this->AddDevice(deviceRgb->GetHardwareId(), deviceRgb);
                LOG_DEBUG(L"Initialized Logitech RGB device");

                auto deviceRgbKey = make_shared<VendorDevice>(shared_from_this());
                deviceRgbKey->SetHardwareId(LOGI_DEVICETYPE_PERKEY_RGB);
                deviceRgbKey->SetDeviceName(L"Full per-key RGB devices");
                deviceRgbKey->SetDeviceType(VendorDeviceType::DeviceKeyboard); // Since we don't know what kind of per-key RGB device this is, we simulate the keyboard
                deviceRgbKey->SetNumberOfLights(1);
                this->AddDevice(deviceRgbKey->GetHardwareId(), deviceRgbKey);
                LOG_DEBUG(L"Initialized Logitech per-key RGB device");
            }
        }

        LFXE_API bool VendorLogitech::EnableDevice(const unsigned int id) {
            // Logitech doesn't support enabling/disabling separate devices natively, so we have to emulate it
            if (this->library->InitializeLibrary()) {
                if (!this->IsDeviceEnabled(id)) {
                    this->enabledDevices[id] = true;
                    this->library->Logi_SetTargetDevice(id);
                    this->library->Logi_SaveCurrentLighting();
                }
                return true;
            }
            return false;
        }

        LFXE_API bool VendorLogitech::DisableDevice(const unsigned int id) {
            // Logitech doesn't support enabling/disabling separate devices natively, so we have to emulate it
            if (this->IsDeviceEnabled(id)) {
                this->enabledDevices[id] = false;
                this->library->Logi_SetTargetDevice(id);
                this->library->Logi_RestoreLighting();

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

        LFXE_API bool VendorLogitech::ResetDevice(const unsigned int id) {
            if (this->IsDeviceEnabled(id)) {
                this->library->Logi_SetTargetDevice(id);
                return this->library->Logi_RestoreLighting();
            }
            return false;
        }

        LFXE_API void VendorLogitech::SetRange(const int outMin, const int outMax, const int inMin, const int inMax) {
            this->rangeOutMin = outMin;
            this->rangeOutMax = outMax;
            this->rangeInMin = inMin;
            this->rangeInMax = inMax;
        }

        LFXE_API void VendorLogitech::SetRestoreLightsWhenOffEnabled(const bool enabled) {
            this->restoreLightsWhenOffEnabled = enabled;
        }

        LFXE_API void VendorLogitech::SetG110WorkaroundEnabled(const bool enabled) {
            this->g110WorkaroundEnabled = enabled;
        }

        LFXE_API bool VendorLogitech::PushColorsToDevice(const unsigned int id, const vector<LightColor>& colors) {
            if (this->IsDeviceEnabled(id) && colors.size() > 0) {
                double red = colors[0].red;
                double green = colors[0].green;
                double blue = colors[0].blue;
                double brightness = colors[0].brightness;

                if (this->restoreLightsWhenOffEnabled && red == 0 && green == 0 && blue == 0 && brightness == 0) {
                    return this->ResetDevice(id);
                }

                if (this->g110WorkaroundEnabled) {
                    double total = red + blue;
                    if (total > 255) {
                        red = red * 255 / total;
                        blue = blue * 255 / total;
                    }
                }

                double divider = (this->rangeOutMax - this->rangeOutMin) / ((this->rangeInMax - this->rangeInMin) / 100.0) / 100.0;
                brightness = ((brightness - this->rangeOutMin) / divider + this->rangeInMin) / 100.0;
                red = (red - this->rangeOutMin) / divider * brightness + this->rangeInMin;
                green = (green - this->rangeOutMin) / divider * brightness + this->rangeInMin;
                blue = (blue - this->rangeOutMin) / divider * brightness + this->rangeInMin;

                if (red > this->rangeInMax) red = this->rangeInMax;
                else if (red < this->rangeInMin) red = this->rangeInMin;
                if (green > this->rangeInMax) green = this->rangeInMax;
                else if (green < this->rangeInMin) green = this->rangeInMin;
                if (blue > this->rangeInMax) blue = this->rangeInMax;
                else if (blue < this->rangeInMin) blue = this->rangeInMin;

                this->library->Logi_SetTargetDevice(id);
                return this->library->Logi_SetLighting((int)red, (int)green, (int)blue);
            }
            return false;
        }

    }
}

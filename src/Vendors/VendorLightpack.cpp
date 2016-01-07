#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "VendorLightpack.h"

// Project includes
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::timelines;
using namespace lightfx::utils;
using namespace lightfx::vendors::libraries;

namespace lightfx {
    namespace vendors {

        LFXE_API void VendorLightpack::InitializeDevices() {
            if (this->IsLibraryAvailable()) {
                auto device = make_shared<VendorDevice>(shared_from_this());
                device->SetHardwareId(0);
                device->SetDeviceName(L"Lightpack");
                device->SetDeviceType(VendorDeviceType::DeviceDisplay);
                device->SetNumberOfLights(0);
                this->AddDevice(device->GetHardwareId(), device);
                LOG_DEBUG(L"Initialized Lightpack device");
            }
        }

        LFXE_API bool VendorLightpack::EnableDevice(const unsigned int id) {
            if (this->library->InitializeLibrary()) {
                if (!this->IsDeviceEnabled(id)) {
                    LightpackStatus status = this->library->Lp_Authorize(this->key);
                    if (status != LightpackStatus::LightpackSuccess) {
                        LOG_ERROR(L"Could not authorize on the API of hardware " + this->GetVendorName() + L" - " + this->GetDevice(id)->GetDeviceName() + L": " +
                            this->library->LightpackStatusToString(status));
                        return false;
                    }
                    status = this->library->Lp_Lock();
                    if (status != LightpackStatus::LightpackSuccess) {
                        LOG_ERROR(L"Could not lock hardware " + this->GetVendorName() + L" - " + this->GetDevice(id)->GetDeviceName() + L": " +
                            this->library->LightpackStatusToString(status));
                        this->library->Lp_Unlock();
                        return false;
                    }
                    // Set the number of lights here
                    this->devices[id]->SetNumberOfLights(this->library->Lp_GetCountLeds());

                    this->enabledDevices[id] = true;
                }
                return true;
            }
            return false;
        }

        LFXE_API bool VendorLightpack::DisableDevice(const unsigned int id) {
            if (this->IsDeviceEnabled(id)) {
                this->library->Lp_Unlock();

                // All devices are disabled (we only have 1), release library
                this->library->ReleaseLibrary();

                this->enabledDevices[id] = false;
                return true;
            }
            return false;
        }

        LFXE_API bool VendorLightpack::ResetDevice(const unsigned int id) {
            // Unsupported
            return false;
        }

        LFXE_API bool VendorLightpack::PushColorsToDevice(const unsigned int id, const vector<LightColor>& colors) {
            if (this->IsDeviceEnabled(id) && colors.size() > 0) {
                vector<LightpackColor> newColors = {};
                for (size_t i = 0; i < colors.size(); ++i) {
                    LightpackColor newColor = {};
                    newColor.index = i + 1;
                    double brightness = colors[i].brightness / 255.0;
                    newColor.red = static_cast<int>(colors[i].red * brightness);
                    newColor.green = static_cast<int>(colors[i].green * brightness);
                    newColor.blue = static_cast<int>(colors[i].blue * brightness);
                    newColors.push_back(newColor);
                }
                return this->library->Lp_SetColors(newColors) == LightpackStatus::LightpackSuccess;
            }
            return false;
        }

    }
}

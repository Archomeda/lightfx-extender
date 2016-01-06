#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "VendorDevice.h"

// Project includes
#include "VendorBase.h"
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::timelines;
using namespace lightfx::vendors;

namespace lightfx {
    namespace vendors {

        LFXE_API shared_ptr<VendorBase> VendorDevice::GetVendor() const {
            return this->vendor.lock();
        }

        LFXE_API void VendorDevice::SetVendor(const shared_ptr<VendorBase>& vendor) {
            this->vendor = vendor;
        }

        LFXE_API unsigned int VendorDevice::GetHardwareId() const {
            return this->hardwareId;
        }

        LFXE_API void VendorDevice::SetHardwareId(const unsigned int hardwareId) {
            this->hardwareId = hardwareId;
        }

        LFXE_API wstring VendorDevice::GetDeviceName() const {
            return this->deviceName;
        }

        LFXE_API void VendorDevice::SetDeviceName(const wstring& deviceName) {
            this->deviceName = deviceName;
        }

        LFXE_API VendorDeviceType VendorDevice::GetDeviceType() const {
            return this->deviceType;
        }

        LFXE_API void VendorDevice::SetDeviceType(const VendorDeviceType& deviceType) {
            this->deviceType = deviceType;
        }

        LFXE_API size_t VendorDevice::GetNumberOfLights() const {
            return this->numberOfLights;
        }

        LFXE_API void VendorDevice::SetNumberOfLights(const size_t numberOfLights) {
            this->numberOfLights = numberOfLights;
            this->lightColors.resize(numberOfLights);
            this->lightData.resize(numberOfLights);
        }

        LFXE_API const VendorDeviceLightData VendorDevice::GetLightData(const size_t lightIndex) const {
            if (lightIndex <= this->numberOfLights && this->lightData.size() > lightIndex) {
                return this->lightData[lightIndex];
            }
            return{};
        }

        LFXE_API void VendorDevice::SetLightData(const size_t lightIndex, const VendorDeviceLightData& lightData) {
            if (lightIndex <= this->numberOfLights) {
                this->lightData[lightIndex] = lightData;
            }
        }

        LFXE_API bool VendorDevice::IsEnabled() const {
            return this->GetVendor()->IsDeviceEnabled(this->hardwareId);
        }
            
        LFXE_API bool VendorDevice::Enable() {
            return this->GetVendor()->EnableDevice(this->hardwareId);
        }

        LFXE_API bool VendorDevice::Disable() {
            return this->GetVendor()->DisableDevice(this->hardwareId);
        }

        LFXE_API bool VendorDevice::Reset() {
            return this->GetVendor()->ResetDevice(this->hardwareId);
        }

        LFXE_API VendorDeviceUpdateStatus VendorDevice::Update(const chrono::milliseconds& timeTick) {
            if (this->IsEnabled()) {
                vector<LightColor> newColors = {};
                VendorDeviceUpdateStatus returnStatus;

                // Get the new colors
                if (this->timelineStartTime.count() > 0) {
                    // There is an active timeline
                    auto timelineTick = (timeTick - this->timelineStartTime).count();
                    if (timelineTick < this->activeTimeline.GetTotalDuration()) {
                        // Active timeline is still going
                        newColors = this->activeTimeline.GetColorAtTime(static_cast<unsigned long>(timelineTick));
                        returnStatus = VendorDeviceUpdateStatus::TimelineBusy;
                    } else {
                        // Active timeline has finished
                        this->timelineStartTime = chrono::milliseconds::zero();
                        newColors = this->activeTimeline.GetColorAtTime(this->activeTimeline.GetTotalDuration());
                        this->activeTimeline = {};
                        returnStatus = VendorDeviceUpdateStatus::TimelineFinished;
                        LOG_DEBUG(L"Timeline finished on " + this->GetVendor()->GetVendorName() + L" - " + this->GetDeviceName());
                    }

                    if (newColors.size() == 0) {
                        LOG_WARNING(L"New colors from active timeline has 0 lights! Ignoring update for " + this->GetVendor()->GetVendorName() + L" - " + this->GetDeviceName());
                        return VendorDeviceUpdateStatus::ErrorSkippedUpdate;
                    }
                } else if (this->activeTimeline.IsValid()) {
                    // Timeline hasn't started yet, but there is a new one
                    this->timelineStartTime = chrono::milliseconds(timeTick);
                    newColors = this->activeTimeline.GetColorAtTime(0);
                    returnStatus = this->activeTimeline.GetTotalDuration() > 0 ? VendorDeviceUpdateStatus::TimelineStarted : VendorDeviceUpdateStatus::TimelineFinished;
                    LOG_DEBUG(L"Starting timeline on " + this->GetVendor()->GetVendorName() + L" - " + this->GetDeviceName() + L": " + this->activeTimeline.ToString());
                } else {
                    return VendorDeviceUpdateStatus::NoActiveTimeline;
                }

                // Push new colors if needed
                if (this->lightColors.size() == newColors.size()) {
                    bool needsUpdate = false;
                    for (size_t i = 0; i < newColors.size(); ++i) {
                        if (this->lightColors[i] != newColors[i]) {
                            needsUpdate = true;
                            break;
                        }
                    }
                    if (needsUpdate) {
                        this->lightColors = newColors;
                        this->PushColorsToDevice(this->lightColors);
                    }
                    return returnStatus;
                } else {
                    LOG_WARNING(L"Amount of new colors from active timeline does not match the actual amount of lights on device! Ignoring update for " + this->GetVendor()->GetVendorName() + L" - " + this->GetDeviceName());
                    return VendorDeviceUpdateStatus::ErrorSkippedUpdate;
                }
            }
            return VendorDeviceUpdateStatus::DeviceNotEnabled;
        }

        LFXE_API Timeline VendorDevice::GetActiveTimeline() const {
            return this->activeTimeline;
        }

        LFXE_API Timeline VendorDevice::GetQueuedTimeline() const {
            return this->queuedTimeline;
        }

        LFXE_API void VendorDevice::SetQueuedTimeline(const Timeline& timeline) {
            this->queuedTimeline = timeline;
        }

        LFXE_API bool VendorDevice::StartQueuedTimeline() {
            if (this->IsEnabled()) {
                this->StopActiveTimeline();
                this->activeTimeline = this->queuedTimeline;
                this->queuedTimeline = {};
                return true;
            }
            return false;
        }

        LFXE_API bool VendorDevice::StopActiveTimeline() {
            if (this->IsEnabled()) {
                this->PushColorsToDevice(this->activeTimeline.GetColorAtTime(this->activeTimeline.GetTotalDuration()));
                this->activeTimeline = {};
                return true;
            }
            return false;
        }

        LFXE_API LightColor VendorDevice::GetCurrentLightColor(const size_t lightIndex) {
            if (this->lightColors.size() > lightIndex) {
                return this->lightColors[lightIndex];
            }
            return{};
        }

        LFXE_API bool VendorDevice::PushColorsToDevice(const vector<LightColor>& colors) {
            if (this->IsEnabled() && colors.size() > 0) {
                return this->GetVendor()->PushColorsToDevice(this->hardwareId, colors);
            }
            return false;
        }

    }
}

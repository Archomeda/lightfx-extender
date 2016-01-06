#pragma once

// Standard includes
#include <chrono>
#include <memory>
#include <string>

// Project includes
#include "../Managers/ChildOfManager.h"
#include "../Timelines/LightColor.h"
#include "../Timelines/Timeline.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class VendorManager;
    }

    namespace vendors {

        class VendorBase;

        enum LFXE_API VendorDeviceType {
            DeviceUnknown = 0x00,
            DeviceNotebook,
            DeviceDesktop,
            DeviceServer,
            DeviceDisplay,
            DeviceMouse,
            DeviceKeyboard,
            DeviceGamepad,
            DeviceSpeaker,
            DeviceOther = 0xFF
        };

        enum LFXE_API VendorDeviceUpdateStatus {
            DeviceNotEnabled,
            NoActiveTimeline,
            ErrorSkippedUpdate,
            TimelineStarted,
            TimelineBusy,
            TimelineFinished
        };

        struct LFXE_API VendorDeviceLightPosition {
            unsigned int x;
            unsigned int y;
            unsigned int z;
        };

        struct LFXE_API VendorDeviceLightData {
            std::wstring name = {};
            VendorDeviceLightPosition position = {};
        };

        class LFXE_API VendorDevice : public managers::ChildOfManager<managers::VendorManager> {

        public:
            VendorDevice(std::shared_ptr<VendorBase> vendor) :
                vendor(vendor) {}
            virtual ~VendorDevice() {}

            std::shared_ptr<VendorBase> GetVendor() const;
            void SetVendor(const std::shared_ptr<VendorBase>& vendor);
            unsigned int GetHardwareId() const;
            void SetHardwareId(const unsigned int hardwareId);

            std::wstring GetDeviceName() const;
            void SetDeviceName(const std::wstring& deviceName);
            VendorDeviceType GetDeviceType() const;
            void SetDeviceType(const VendorDeviceType& deviceType);

            size_t GetNumberOfLights() const;
            void SetNumberOfLights(const size_t numberOfLights);
            const VendorDeviceLightData GetLightData(const size_t lightIndex) const;
            void SetLightData(const size_t lightIndex, const VendorDeviceLightData& lightData);

            bool IsEnabled() const;
            bool Enable();
            bool Disable();
            bool Reset();
            VendorDeviceUpdateStatus Update(const std::chrono::milliseconds& timeTick);

            timelines::Timeline GetActiveTimeline() const;
            timelines::Timeline GetQueuedTimeline() const;
            void SetQueuedTimeline(const timelines::Timeline& timeline);
            bool StartQueuedTimeline();
            bool StopActiveTimeline();

            timelines::LightColor GetCurrentLightColor(const size_t lightIndex);
            bool PushColorsToDevice(const std::vector<timelines::LightColor>& colors);

        private:
            std::weak_ptr<VendorBase> vendor;
            unsigned int hardwareId;
            std::wstring deviceName = {};
            VendorDeviceType deviceType = DeviceUnknown;

            size_t numberOfLights = 0;
            std::vector<timelines::LightColor> lightColors = {};
            std::vector<VendorDeviceLightData> lightData = {};

            timelines::Timeline activeTimeline;
            timelines::Timeline queuedTimeline;
            std::chrono::milliseconds timelineStartTime;
        };

    }
}

#pragma warning(pop)

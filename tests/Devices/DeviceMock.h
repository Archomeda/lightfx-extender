#pragma once

// Standard includes
#include <string>

// Project includes
#include "../../src/Devices/Device.h"


namespace lightfx_tests {
    namespace devices {

        class DeviceMock : public lightfx::devices::Device {

        public:
            using Device::IsEnabled;
            using Device::IsInitialized;

            using Device::Enable;
            using Device::Disable;

            using Device::Initialize;
            using Device::Release;
            using Device::Update;
            using Device::Reset;

            using Device::GetActiveTimeline;
            using Device::GetQueuedTimeline;
            using Device::GetRecentTimeline;
            using Device::QueueTimeline;
            using Device::NotifyUpdate;

            virtual const std::wstring GetDeviceName() override { return L"DeviceMock"; }
            virtual const lightfx::devices::DeviceType GetDeviceType() override { return lightfx::devices::DeviceType::DeviceOther; }

            using Device::GetNumberOfLights;
            using Device::GetLightData;
            using Device::SetLightData;

        protected:
            virtual bool PushColorToDevice(const std::vector<lightfx::timelines::LightColor>& colors) override { return false; }

        };

    }
}

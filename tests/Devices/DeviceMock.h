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

            virtual const std::wstring GetDeviceName() override { return L"DeviceMock"; }
            virtual const unsigned char GetDeviceType() override { return 0; }

        };

    }
}

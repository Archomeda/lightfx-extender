#pragma once

// Windows includes
#include "Device.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace devices {

        class LFXE_API DeviceLogitech : public Device {

        public: 
            void SetRange(const int outMin, const int outMax, const int inMin, const int inMax);

            virtual bool Initialize() override;
            virtual bool Release() override;

            virtual bool PushColorToDevice() override;

            virtual const std::wstring GetDeviceName() override { return L"Logitech"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::Keyboard; }

        private:
            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 100;

        };

    }
}

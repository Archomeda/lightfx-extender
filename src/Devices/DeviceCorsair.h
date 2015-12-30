#pragma once

// Windows includes
#include "Device.h"

// API exports
#include "../Common/ApiExports.h"

#include "CUESDK.h"


namespace lightfx {
    namespace devices {

        class LFXE_API DeviceCorsair : public Device {

        public:
            void SetRange(const int outMin, const int outMax, const int inMin, const int inMax);

            virtual bool Initialize() override;
            virtual bool Enable() override;

            virtual const std::wstring GetDeviceName() override { return L"Corsair"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceKeyboard; }

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

        private:
            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 255;
            CorsairLedPositions* ledPositions;

            const char* toString(CorsairError error);
        };

    }
}

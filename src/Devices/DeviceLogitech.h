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
            void SetRestoreLightsOnNullEnabled(const bool enabled);
            void SetG110WorkaroundEnabled(const bool enabled);


            virtual bool Initialize() override;
            virtual bool Enable() override;
            virtual bool Disable() override;

            virtual const std::wstring GetDeviceName() override { return L"Logitech"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceKeyboard; }

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

        private:
            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 100;

            bool RestoreLightsOnNullEnabled = false;
            bool g110WorkaroundEnabled = false;

        };

    }
}

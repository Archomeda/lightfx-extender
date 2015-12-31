#pragma once

// Windows includes
#include "Device.h"

// API exports
#include "../Common/ApiExports.h"

#include <Windows.h>
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"

namespace lightfx {
    namespace devices {

        class LFXE_API DeviceRazer : public Device {

        public:
            void SetHardware(const bool keyboard, const bool mouse, const bool headset, const bool mousepad, const bool keypad);

            virtual bool Initialize() override;
            virtual bool Enable() override;
            virtual bool Disable() override;

            virtual const std::wstring GetDeviceName() override { return L"Razer"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceKeyboard; }

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

        private:
            bool useWithKeyboard = true;
            bool useWithMouse = true;
            bool useWithHeadset = true;
            bool useWithMousepad = true;
            bool useWithKeypad = true;

            bool IsDeviceConnected(const RZDEVICEID DeviceId);
        };

    }
}

#pragma once

// Project includes
#include "Device.h"
#include "Proxies/RzChromaSDKProxy.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace devices {

        class LFXE_API DeviceRazer : public Device {

        public:
            void SetHardware(const bool keyboard, const bool mouse, const bool headset, const bool mousepad, const bool keypad);

            virtual bool Initialize() override;
            virtual bool Release() override;

            virtual bool Enable() override;
            virtual bool Disable() override;

            virtual const std::wstring GetDeviceName() override { return L"Razer"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceKeyboard; }

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

        private:
            std::unique_ptr<proxies::RzChromaSDKProxy> library;

            bool useWithKeyboard = true;
            bool useWithMouse = true;
            bool useWithHeadset = true;
            bool useWithMousepad = true;
            bool useWithKeypad = true;

            bool IsDeviceConnected(const RZDEVICEID DeviceId);
        };

    }
}

#pragma warning(pop)

#pragma once

// 3rd party includes
#include "CUESDK.h"

// Project includes
#include "Device.h"
#include "Proxies/CUESDKProxy.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace devices {

        class LFXE_API DeviceCorsair : public Device {

        public:
            void SetRange(const int outMin, const int outMax, const int inMin, const int inMax);

            virtual bool Initialize() override;
            virtual bool Release() override;

            virtual bool Enable() override;

            virtual const std::wstring GetDeviceName() override { return L"Corsair"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceKeyboard; }

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

        private:
            std::unique_ptr<proxies::CUESDKProxy> library;

            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 255;
            CorsairLedPositions* ledPositions;
        };

    }
}

#pragma warning(pop)

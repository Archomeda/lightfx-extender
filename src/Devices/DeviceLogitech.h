#pragma once

// Project includes
#include "DeviceBase.h"


namespace lightfx {
    namespace devices {

        class DeviceLogitech : public DeviceBase {

        public:
            void SetRange(const int outMin, const int outMax, const int inMin, const int inMax);

            virtual bool Initialize() override;
            virtual bool Release() override;
            virtual bool Update() override;

            virtual std::wstring GetDeviceName() override;
            virtual unsigned char GetDeviceType() override;

        private:
            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 100;

        };

    }
}

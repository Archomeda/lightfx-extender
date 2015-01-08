#pragma once
#include "DeviceBase.h"

namespace lightfx {
    namespace devices {

        class DeviceLogitech : public DeviceBase {

        public:
            void SetRange(const int outMin, const int outMax, const int inMin, const int inMax);

            virtual LFX_RESULT Initialize() override;
            virtual LFX_RESULT Release() override;
            virtual LFX_RESULT Update() override;

            virtual LFX_RESULT GetDeviceInfo(std::string& description, unsigned char& type) override;

        private:
            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 100;

        };

    }
}

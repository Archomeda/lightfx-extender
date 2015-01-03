#pragma once
#include "Device.h"

namespace lightfx {

    class DeviceLogitech : public Device {

    public:
        virtual LFX_RESULT Initialize() override;
        virtual LFX_RESULT Release() override;
        virtual LFX_RESULT Update() override;

        virtual LFX_RESULT GetDeviceInfo(std::string& description, unsigned char& type) override;

    };

}

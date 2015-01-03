#include <Windows.h> // Windows.h is needed for some declarations inside LogitechLEDLib.h
#include "LogitechLEDLib.h"

#include "DeviceLogitech.h"

using namespace std;

#define DEVICENAME "Logitech LED"
#define DEVICETYPE LFX_DEVTYPE_KEYBOARD

namespace lightfx {

    void DeviceLogitech::SetRange(const int outMin, const int outMax, const int inMin, const int inMax) {
        this->rangeOutMin = outMin;
        this->rangeOutMax = outMax;
        this->rangeInMin = inMin;
        this->rangeInMax = inMax;
    }

    LFX_RESULT DeviceLogitech::Initialize() {
        if (!this->Initialized()) {
            if (LogiLedInit()) {
                this->Lights.clear();

                // TODO: Customizable Logitech config
                DeviceLightData light;
                light.Description = "";
                light.Position = LFX_POSITION();
                this->Lights.push_back(light);

                return Device::Initialize();
            }
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT DeviceLogitech::Release() {
        if (this->Initialized()) {
            LogiLedShutdown();
            return Device::Release();
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT DeviceLogitech::Update() {
        auto result = Device::Update();

        if (result != LFX_SUCCESS) {
            return result;
        }

        double divider = (this->rangeOutMax - this->rangeOutMin) / ((this->rangeInMax - this->rangeInMin) / 100.0) / 100.0;
        double brightness = this->CurrentPrimaryColor[0].brightness / 255.0;
        double red = (this->CurrentPrimaryColor[0].red - this->rangeOutMin) / divider * brightness + this->rangeInMin;
        double green = (this->CurrentPrimaryColor[0].green - this->rangeOutMin) / divider * brightness + this->rangeInMin;
        double blue = (this->CurrentPrimaryColor[0].blue - this->rangeOutMin) / divider * brightness + this->rangeInMin;

        return LogiLedSetLighting((int)red, (int)green, (int)blue) ? LFX_SUCCESS : LFX_FAILURE;
    }

    LFX_RESULT DeviceLogitech::GetDeviceInfo(string& description, unsigned char& type) {
        description = DEVICENAME;
        type = DEVICETYPE;
        return LFX_SUCCESS;
    }

}

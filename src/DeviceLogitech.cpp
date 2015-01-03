#include <Windows.h> // Windows.h is needed for some declarations inside LogitechLEDLib.h
#include "LogitechLEDLib.h"

#include "DeviceLogitech.h"

using namespace std;

#define DEVICENAME "Logitech LED"
#define DEVICETYPE LFX_DEVTYPE_KEYBOARD

namespace lightfx {

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

        double brightness = this->CurrentPrimaryColor[0].brightness / 255.0;
        double red = this->CurrentPrimaryColor[0].red / 2.55 * brightness;
        double green = this->CurrentPrimaryColor[0].green / 2.55 * brightness;
        double blue = this->CurrentPrimaryColor[0].blue / 2.55 * brightness;

        return LogiLedSetLighting((int)red, (int)green, (int)blue) ? LFX_SUCCESS : LFX_FAILURE;
    }

    LFX_RESULT DeviceLogitech::GetDeviceInfo(string& description, unsigned char& type) {
        description = DEVICENAME;
        type = DEVICETYPE;
        return LFX_SUCCESS;
    }

}

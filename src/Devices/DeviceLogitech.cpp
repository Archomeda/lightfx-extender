#include "DeviceLogitech.h"

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h> // Windows.h is needed for some declarations inside LogitechLEDLib.h

// 3rd party includes
#include "LogitechLEDLib.h"


using namespace std;

#define DEVICENAME L"Logitech"
#define DEVICETYPE LFX_DEVTYPE_KEYBOARD

namespace lightfx {
    namespace devices {

        void DeviceLogitech::SetRange(const int outMin, const int outMax, const int inMin, const int inMax) {
            this->rangeOutMin = outMin;
            this->rangeOutMax = outMax;
            this->rangeInMin = inMin;
            this->rangeInMax = inMax;
        }

        bool DeviceLogitech::Initialize() {
            if (!this->IsInitialized()) {
                if (LogiLedInit()) {
                    this->Lights.clear();

                    // TODO: Customizable Logitech config
                    DeviceLight light;
                    this->Lights.push_back(light);

                    return DeviceBase::Initialize();
                }
            }
            return true;
        }

        bool DeviceLogitech::Release() {
            if (this->IsInitialized()) {
                LogiLedShutdown();
                return DeviceBase::Release();
            }
            return true;
        }

        bool DeviceLogitech::Update() {
            if (!this->IsEnabled()) {
                return true;
            }

            if (!DeviceBase::Update()) {
                return false;
            }

            double divider = (this->rangeOutMax - this->rangeOutMin) / ((this->rangeInMax - this->rangeInMin) / 100.0) / 100.0;
            double brightness = this->CurrentPrimaryColor[0].brightness / 255.0;
            double red = (this->CurrentPrimaryColor[0].red - this->rangeOutMin) / divider * brightness + this->rangeInMin;
            double green = (this->CurrentPrimaryColor[0].green - this->rangeOutMin) / divider * brightness + this->rangeInMin;
            double blue = (this->CurrentPrimaryColor[0].blue - this->rangeOutMin) / divider * brightness + this->rangeInMin;

            return LogiLedSetLighting((int)red, (int)green, (int)blue);
        }

        wstring DeviceLogitech::GetDeviceName() {
            return DEVICENAME;
        }

        unsigned char DeviceLogitech::GetDeviceType() {
            return DEVICETYPE;
        }

    }
}

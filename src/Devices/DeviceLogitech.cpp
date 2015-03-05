#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceLogitech.h"

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h> // Windows.h is needed for some declarations inside LogitechLEDLib.h

// 3rd party includes
#include "LogitechLEDLib.h"


using namespace std;

namespace lightfx {
    namespace devices {

        LFXE_API void DeviceLogitech::SetRange(const int outMin, const int outMax, const int inMin, const int inMax) {
            this->rangeOutMin = outMin;
            this->rangeOutMax = outMax;
            this->rangeInMin = inMin;
            this->rangeInMax = inMax;
        }

        LFXE_API bool DeviceLogitech::Initialize() {
            if (!this->IsInitialized()) {
                if (Device::Initialize()) {
                    // Just do an initial pass to set how many LEDs there are available
                    // TODO: Support more Logitech customization (e.g. Logitech G910 single-key colors)
                    this->SetNumberOfLights(1);
                    this->SetLightData(0, LightData());

                    this->Reset();
                    return true;
                }
            }
            return false;
        }

        LFXE_API bool DeviceLogitech::Enable() {
            if (!this->IsEnabled()) {
                if (Device::Enable()) {
                    if (LogiLedInit()) {
                        this->Reset();
                        return true;
                    }
                }
            }
            return false;
        }

        LFXE_API bool DeviceLogitech::Disable() {
            if (this->IsEnabled()) {
                if (Device::Disable()) {
                    LogiLedShutdown();
                    return true;
                }
            }
            return false;
        }

        LFXE_API bool DeviceLogitech::PushColorToDevice() {
            LightColor color = this->CurrentLightAction.GetCurrentColor(0);
            double divider = (this->rangeOutMax - this->rangeOutMin) / ((this->rangeInMax - this->rangeInMin) / 100.0) / 100.0;
            double brightness = color.brightness / 255.0;
            double red = (color.red - this->rangeOutMin) / divider * brightness + this->rangeInMin;
            double green = (color.green - this->rangeOutMin) / divider * brightness + this->rangeInMin;
            double blue = (color.blue - this->rangeOutMin) / divider * brightness + this->rangeInMin;

            return LogiLedSetLighting((int)red, (int)green, (int)blue);
        }

    }
}
#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceLogitech.h"

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h> // Windows.h is needed for some declarations inside LogitechLEDLib.h

// 3rd party includes
#include "LogitechLEDLib.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + message)

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
    namespace devices {

        LFXE_API void DeviceLogitech::SetRange(const int outMin, const int outMax, const int inMin, const int inMax) {
            this->rangeOutMin = outMin;
            this->rangeOutMax = outMax;
            this->rangeInMin = inMin;
            this->rangeInMax = inMax;
        }

        LFXE_API void DeviceLogitech::SetRestoreLightsOnNullEnabled(const bool enabled) {
            this->RestoreLightsOnNullEnabled = enabled;
        }

        LFXE_API void DeviceLogitech::SetG110WorkaroundEnabled(const bool enabled) {
            this->g110WorkaroundEnabled = enabled;
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
            this->SetInitialized(false);
            return false;
        }

        LFXE_API bool DeviceLogitech::Enable() {
            if (!this->IsEnabled()) {
                if (Device::Enable()) {
                    if (LogiLedInit()) {
                        this->Reset();
                        LogiLedSaveCurrentLighting();
                        return true;
                    } else {
                        LOG(LogLevel::Error, L"Could not enable Logitech, make sure that Logitech Gaming Software is running and that it's at least at version 8.57.145");
                    }
                }
            }
            this->SetEnabled(false);
            return false;
        }

        LFXE_API bool DeviceLogitech::Disable() {
            if (this->IsEnabled()) {
                if (Device::Disable()) {
                    LogiLedRestoreLighting();
                    LogiLedShutdown();
                    return true;
                }
            }
            this->SetEnabled(true);
            return false;
        }

        LFXE_API bool DeviceLogitech::PushColorToDevice(const vector<LightColor>& colors) {
            double red = colors[0].red;
            double green = colors[0].green;
            double blue = colors[0].blue;
            double alpha = colors[0].brightness;

            if (this->RestoreLightsOnNullEnabled && red == 0.0 && green == 0.0 && blue == 0.0 && alpha == 0.0)
            {
                return LogiLedRestoreLighting();
            }

            if (this->g110WorkaroundEnabled) {
                double total = red + blue;
                if (total > 255) {
                    red = red * 255 / total;
                    blue = blue * 255 / total;
                }
            }

            double divider = (this->rangeOutMax - this->rangeOutMin) / ((this->rangeInMax - this->rangeInMin) / 100.0) / 100.0;
            double brightness = colors[0].brightness / 255.0;
            red = (red - this->rangeOutMin) / divider * brightness + this->rangeInMin;
            green = (green - this->rangeOutMin) / divider * brightness + this->rangeInMin;
            blue = (blue - this->rangeOutMin) / divider * brightness + this->rangeInMin;

            LOG(LogLevel::Debug, L"Update color to (" + to_wstring(red) + L"," + to_wstring(green) + L"," + to_wstring(blue) + L")");

            return LogiLedSetLighting((int)red, (int)green, (int)blue);
        }

    }
}
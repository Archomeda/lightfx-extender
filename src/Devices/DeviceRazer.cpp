#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceRazer.h"

// Standard includes
#include <tchar.h>

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::devices::proxies;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Mousepad;
using namespace ChromaSDK::Headset;

namespace lightfx {
    namespace devices {

        LFXE_API void DeviceRazer::SetHardware(const bool keyboard, const bool mouse, const bool headset, const bool mousepad, const bool keypad) {
            this->useWithKeyboard = keyboard;
            this->useWithMouse = mouse;
            this->useWithHeadset = headset;
            this->useWithMousepad = mousepad;
            this->useWithKeypad = keypad;
        }

        LFXE_API bool DeviceRazer::Initialize() {
            if (!this->IsInitialized()) {
                if (Device::Initialize()) {
                    // Load the library first
                    this->library = unique_ptr<RzChromaSDKProxy>(new RzChromaSDKProxy);
                    if (!this->library->Load()) {
                        LOG_ERROR(L"Failed to access the RzChromaSDK library");
                        this->SetInitialized(false);
                        return false;
                    }

                    // Just do an initial pass to set how many LEDs there are available
                    this->SetNumberOfLights(1);
                    this->SetLightData(0, LightData());

                    this->Reset();
                } else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceRazer::Release() {
            if (this->IsInitialized()) {
                if (Device::Release()) {
                    bool result = this->library->Unload();
                    this->SetInitialized(!result);
                    return result;
                } else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceRazer::Enable() {
            if (!this->IsEnabled()) {
                if (Device::Enable()) {
                    RZRESULT result = this->library->RzInit();
                    if (result == RZRESULT_SUCCESS) {
                            this->Reset();
                    } else {
                        LOG_ERROR(L"Could not enable Razor device, initialization error: " + this->library->RzResultToString(result));
                        this->SetEnabled(false);
                        return false;
                    }
                } else {
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceRazer::Disable() {
            if (this->IsEnabled()) {
                if (Device::Disable()) {
                    RZRESULT result = this->library->RzUnInit();
                    if (result != RZRESULT_SUCCESS) {
                        LOG_ERROR(L"Could not disable Razor device, uninitialization error: " + this->library->RzResultToString(result));
                        this->SetEnabled(true);
                        return false;
                    }
                } else {
                    this->SetEnabled(true);
                    return false;
                }
            }
            return true;
        }

        LFXE_API bool DeviceRazer::PushColorToDevice(const vector<LightColor>& colors) {
            double red = colors[0].red;
            double green = colors[0].green;
            double blue = colors[0].blue;
            double alpha = colors[0].brightness;

            int updated_red = (int)(red * (alpha / 255.0));
            int updated_green = (int)(green * (alpha / 255.0));
            int updated_blue = (int)(blue * (alpha / 255.0));

            COLORREF Color = RGB(updated_red, updated_green, updated_blue);

            LOG_DEBUG(L"Update color to (" + to_wstring(updated_red) + L"," + to_wstring(updated_green) + L"," + to_wstring(updated_blue) + L")");

            bool keyboardresult = true;
            bool mouseresult = true;
            bool headsetresult = true;
            bool mousepadresult = true;
            bool keypadresult = true;

            //Keyboard
            if (this->useWithKeyboard)
            {
                ChromaSDK::Keyboard::STATIC_EFFECT_TYPE Effect = {};
                Effect.Color = Color;

                RZRESULT Result = this->library->RzCreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &Effect, NULL);
                keyboardresult = Result == RZRESULT_SUCCESS;
            }

            //Mouse
            if (this->useWithMouse)
            {
                ChromaSDK::Mouse::STATIC_EFFECT_TYPE Effect = {};
                Effect.Color = Color;
                
                RZRESULT Result = this->library->RzCreateMouseEffect(ChromaSDK::Mouse::CHROMA_STATIC, &Effect, NULL);
                mouseresult = Result == RZRESULT_SUCCESS;
            }

            //Headset
            if (this->useWithHeadset)
            {
                ChromaSDK::Headset::STATIC_EFFECT_TYPE Effect = {};
                Effect.Color = Color;

                RZRESULT Result = this->library->RzCreateHeadsetEffect(ChromaSDK::Headset::CHROMA_STATIC, &Effect, NULL);
                headsetresult = Result == RZRESULT_SUCCESS;
            }

            //Mousepad
            if (this->useWithMousepad)
            {
                ChromaSDK::Mousepad::STATIC_EFFECT_TYPE Effect = {};
                Effect.Color = Color;

                RZRESULT Result = this->library->RzCreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_STATIC, &Effect, NULL);
                mousepadresult = Result == RZRESULT_SUCCESS;
            }

            //Keypadpad
            if (this->useWithKeypad)
            {
                ChromaSDK::Keypad::STATIC_EFFECT_TYPE Effect = {};
                Effect.Color = Color;

                RZRESULT Result = this->library->RzCreateKeypadEffect(ChromaSDK::Keypad::CHROMA_STATIC, &Effect, NULL);
                keypadresult = Result == RZRESULT_SUCCESS;
            }

            return keyboardresult && headsetresult && mouseresult && mousepadresult && keypadresult;
        }
    }
}

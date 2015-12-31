#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceRazer.h"

// Standard includes
#include <tchar.h>

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1
#define MOUSEMAT_DEVICES    2
#define MOUSE_DEVICES       3
#define HEADSET_DEVICES     4
#define KEYPAD_DEVICES      5

#define LOG(logLevel, message) LOG_(logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + message)

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Mousepad;
using namespace ChromaSDK::Headset;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

HMODULE m_ChromaSDKModule = NULL;
INIT Init = NULL;
UNINIT UnInit = NULL;
CREATEEFFECT CreateEffect = NULL;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
CREATEMOUSEEFFECT CreateMouseEffect = NULL;
CREATEHEADSETEFFECT CreateHeadsetEffect = NULL;
CREATEMOUSEPADEFFECT CreateMousepadEffect = NULL;
CREATEKEYPADEFFECT CreateKeypadEffect = NULL;
SETEFFECT SetEffect = NULL;
DELETEEFFECT DeleteEffect = NULL;
QUERYDEVICE QueryDevice = NULL;

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

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
                    // Just do an initial pass to set how many LEDs there are available
                    this->SetNumberOfLights(1);
                    this->SetLightData(0, LightData());

                    this->Reset();
                    return true;
                }
            }
            this->SetInitialized(false);
            return false;
        }

        LFXE_API bool DeviceRazer::Enable() {
            if (!this->IsEnabled()) {
                if (Device::Enable()) {

                    if (m_ChromaSDKModule == NULL)
                    {
                        m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
                        if (m_ChromaSDKModule == NULL)
                        {
                            LOG(LogLevel::Error, L"Failed initializing Razer");
                        }
                    }

                    if (Init == NULL)
                    {
                        RZRESULT Result = RZRESULT_INVALID;
                        Init = (INIT)GetProcAddress(m_ChromaSDKModule, "Init");
                        if (Init)
                        {
                            Result = Init();
                            if (Result == RZRESULT_SUCCESS)
                            {
                                CreateEffect = (CREATEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateEffect");
                                CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect");
                                CreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMouseEffect");
                                CreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateHeadsetEffect");
                                CreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMousepadEffect");
                                CreateKeypadEffect = (CREATEKEYPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeypadEffect");
                                SetEffect = (SETEFFECT)GetProcAddress(m_ChromaSDKModule, "SetEffect");
                                DeleteEffect = (DELETEEFFECT)GetProcAddress(m_ChromaSDKModule, "DeleteEffect");
                                QueryDevice = (QUERYDEVICE)GetProcAddress(m_ChromaSDKModule, "QueryDevice");
                                
                                if (CreateEffect &&
                                    CreateKeyboardEffect &&
                                    CreateMouseEffect &&
                                    CreateHeadsetEffect &&
                                    CreateMousepadEffect &&
                                    CreateKeypadEffect &&
                                    SetEffect &&
                                    DeleteEffect && 
                                    QueryDevice)
                                {
                                    //Check if any Razer device is connected
                                    if (IsDeviceConnected(BLACKWIDOW_CHROMA) ||
                                        IsDeviceConnected(BLACKWIDOW_CHROMA_TE) ||
                                        IsDeviceConnected(DEATHSTALKER_CHROMA) ||
                                        IsDeviceConnected(OVERWATCH_KEYBOARD) ||
                                        IsDeviceConnected(DEATHADDER_CHROMA) ||
                                        IsDeviceConnected(MAMBA_CHROMA_TE) ||
                                        IsDeviceConnected(DIAMONDBACK_CHROMA) ||
                                        IsDeviceConnected(MAMBA_CHROMA) ||
                                        IsDeviceConnected(NAGA_EPIC_CHROMA) ||
                                        IsDeviceConnected(OROCHI_CHROMA) ||
                                        IsDeviceConnected(KRAKEN71_CHROMA) ||
                                        IsDeviceConnected(FIREFLY_CHROMA) ||
                                        IsDeviceConnected(TARTARUS_CHROMA) ||
                                        IsDeviceConnected(ORBWEAVER_CHROMA)
                                        )
                                    {
                                        this->Reset();
                                        return true;
                                    }
                                    else
                                    {
                                        LOG(LogLevel::Error, L"No Razer devices connected");
                                    }
                                }
                                else
                                {
                                    LOG(LogLevel::Error, L"Failed initializing Razer Functions");
                                }
                            }
                            else
                            {
                                LOG(LogLevel::Error, L"Failed initializing Razer");
                            }
                        }
                    }
                }
            }
            this->SetEnabled(false);
            return false;
        }

        LFXE_API bool DeviceRazer::Disable() {
            if (this->IsEnabled()) {
                if (Device::Disable()) {

                    if (m_ChromaSDKModule != NULL)
                    {
                        RZRESULT Result = RZRESULT_INVALID;
                        UNINIT UnInit = (UNINIT)GetProcAddress(m_ChromaSDKModule, "UnInit");
                        if (UnInit)
                        {
                            UnInit();
                        }

                        FreeLibrary(m_ChromaSDKModule);
                        m_ChromaSDKModule = NULL;
                    }
                    return true;
                }
            }
            this->SetEnabled(true);
            return false;
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

            LOG(LogLevel::Debug, L"Update color to (" + to_wstring(updated_red) + L"," + to_wstring(updated_green) + L"," + to_wstring(updated_blue) + L")");

            bool keyboardresult = true;
            bool mouseresult = true;
            bool headsetresult = true;
            bool mousepadresult = true;
            bool keypadresult = true;

            //Keyboard
            if (CreateKeyboardEffect && this->useWithKeyboard)
            {
                ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect = {};
                for (UINT row = 0; row<ChromaSDK::Keyboard::MAX_ROW; row++)
                    for (UINT col = 0; col<ChromaSDK::Keyboard::MAX_COLUMN; col++)
                        Effect.Color[row][col] = Color;

                RZRESULT Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &Effect, NULL);

                keyboardresult = Result == RZRESULT_SUCCESS;
            }

            //Mouse
            if (CreateMouseEffect && this->useWithMouse)
            {
                ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE2 Effect = {};
                for (int i = 0; i<Mouse::MAX_ROW; i++)
                    for (int j = 0; j<Mouse::MAX_COLUMN; j++)
                        Effect.Color[i][j] = Color;

                RZRESULT Result = CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM2, &Effect, NULL);

                mouseresult = Result == RZRESULT_SUCCESS;
            }

            //Headset
            if (CreateHeadsetEffect && this->useWithHeadset)
            {
                ChromaSDK::Headset::STATIC_EFFECT_TYPE Effect = {};
                Effect.Color = Color;

                RZRESULT Result = CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_STATIC, &Effect, NULL);
                headsetresult = Result == RZRESULT_SUCCESS;
            }

            //Mousepad
            if (CreateMousepadEffect && this->useWithMousepad)
            {
                ChromaSDK::Mousepad::STATIC_EFFECT_TYPE Effect = {};
                Effect.Color = Color;

                RZRESULT Result = CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_STATIC, &Effect, NULL);
                mousepadresult = Result == RZRESULT_SUCCESS;
            }

            //Keypadpad
            if (CreateKeypadEffect && this->useWithKeypad)
            {
                ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE Effect = {};

                for (UINT i = 0; i<ChromaSDK::Keypad::MAX_ROW; i++)
                    for (UINT j = 0; j<ChromaSDK::Keypad::MAX_COLUMN; j++)
                        Effect.Color[i][j] = Color;

                RZRESULT Result = CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_STATIC, &Effect, NULL);
                keypadresult = Result == RZRESULT_SUCCESS;
            }

            return keyboardresult && headsetresult && mouseresult && mousepadresult && keypadresult;
        }

        LFXE_API bool DeviceRazer::IsDeviceConnected(const RZDEVICEID DeviceId)
        {
            if (QueryDevice != NULL)
            {
                ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
                RZRESULT Result = QueryDevice(DeviceId, DeviceInfo);

                if (Result == RZRESULT_SUCCESS)
                    return DeviceInfo.Connected == TRUE;
                else
                    return false;
            }

            return false;
        }
    }
}

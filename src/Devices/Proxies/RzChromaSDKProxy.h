#pragma once

// Standard includes
#include <string>

// Windows includes
#include "../../Common/Windows.h"
#include <Windows.h>

// 3rd party includes
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"

// Project includes
#include "DeviceLibraryProxy.h"

// API exports
#include "../../Common/ApiExports.h"


// Explicit linking is used here.
//! Keep in mind that we have to manually update this every time the SDK has been updated.

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1
#define MOUSEMAT_DEVICES    2
#define MOUSE_DEVICES       3
#define HEADSET_DEVICES     4
#define KEYPAD_DEVICES      5


typedef RZRESULT(*RZINIT)(void);
typedef RZRESULT(*RZUNINIT)(void);
typedef RZRESULT(*RZCREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*RZCREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*RZCREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*RZCREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*RZCREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*RZCREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*RZSETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*RZDELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*RZQUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

#ifdef _WIN64
#define RZ_DLL_NAME         "RzChromaSDK64.dll"
#else
#define RZ_DLL_NAME         "RzChromaSDK.dll"
#endif

#define RZ_DLL_INIT "Init"
#define RZ_DLL_UNINIT "UnInit"
#define RZ_DLL_CREATEEFFECT "CreateEffect"
#define RZ_DLL_CREATEKEYBOARDEFFECT "CreateKeyboardEffect"
#define RZ_DLL_CREATEHEADSETEFFECT "CreateHeadsetEffect"
#define RZ_DLL_CREATEMOUSEPADEFFECT "CreateMousepadEffect"
#define RZ_DLL_CREATEMOUSEEFFECT "CreateMouseEffect"
#define RZ_DLL_CREATEKEYPADEFFECT "CreateKeypadEffect"
#define RZ_DLL_SETEFFECT "SetEffect"
#define RZ_DLL_DELETEEFFECT "DeleteEffect"
#define RZ_DLL_QUERYDEVICE "QueryDevice"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace devices {
        namespace proxies {

            class LFXE_API RzChromaSDKProxy : DeviceLibraryProxy {

            public:
                RzChromaSDKProxy() {}

                virtual bool Load() override;
                virtual bool Unload() override;

                using DeviceLibraryProxy::IsLoaded;

                std::wstring RzResultToString(const RZRESULT result);

                // RZ function declarations
                RZINIT RzInit;
                RZUNINIT RzUnInit;
                RZCREATEEFFECT RzCreateEffect;
                RZCREATEKEYBOARDEFFECT RzCreateKeyboardEffect;
                RZCREATEHEADSETEFFECT RzCreateHeadsetEffect;
                RZCREATEMOUSEPADEFFECT RzCreateMousepadEffect;
                RZCREATEMOUSEEFFECT RzCreateMouseEffect;
                RZCREATEKEYPADEFFECT RzCreateKeypadEffect;
                RZSETEFFECT RzSetEffect;
                RZDELETEEFFECT RzDeleteEffect;
                RZQUERYDEVICE RzQueryDevice;

            protected:
                bool ReleaseLibrary();

            private:
                HINSTANCE hInstance = NULL;

            };

        }
    }
}

#pragma warning(pop)

#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

// Standard includes
#include <string>
#include <memory>

// 3rd party includes
#include "LFX2.h"

// Project includes
#include "lightFXExtender.h"
#include "Managers/DeviceManager.h"
#include "Utils/String.h"


using namespace std;
using namespace lightfx;
using namespace lightfx::devices;
using namespace lightfx::utils;


#pragma region Converters

unsigned char DeviceTypeToLfxDeviceType(const DeviceType deviceType) {
    switch (deviceType) {
    case DeviceType::DeviceDisplay:
        return LFX_DEVTYPE_DISPLAY;
    case DeviceType::DeviceKeyboard:
        return LFX_DEVTYPE_KEYBOARD;
    case DeviceType::DeviceOther:
        return LFX_DEVTYPE_OTHER;
    case DeviceType::DeviceUnknown:
    default:
        return LFX_DEVTYPE_UNKNOWN;
    }
}

LFX_POSITION DeviceLightPositionToLfxPosition(const DeviceLightPosition position) {
    return LFX_POSITION{ position.x, position.y, position.z };
}

LFX_COLOR LightColorToLfxColor(const LightColor color) {
    return LFX_COLOR{ color.red, color.green, color.blue, color.brightness };
}

LFX_COLOR IntToLfxColor(const unsigned int color) {
    LFX_COLOR c;
    c.blue = (color >> 24) & 0xFF;
    c.green = (color >> 16) & 0xFF;
    c.red = (color >> 8) & 0xFF;
    c.brightness = color & 0xFF;
    return c;
}

LightColor LfxColorToLightColor(const LFX_COLOR color) {
    return LightColor{ color.red, color.green, color.blue, color.brightness };
}

LightColor IntToLightColor(const unsigned int color) {
    return LfxColorToLightColor(IntToLfxColor(color));
}

#pragma endregion


shared_ptr<LightFXExtender> lightFXExtender = make_shared<LightFXExtender>();
unsigned int timing = 200;

#ifdef __cplusplus
extern "C" {
#endif

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Initialize() {
        lightFXExtender->Initialize();
        lightFXExtender->Start();
        return lightFXExtender->GetDeviceManager()->GetChildrenCount() > 0 ? LFX_SUCCESS : LFX_ERROR_NODEVS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Release() {
        lightFXExtender->Stop();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Reset() {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (deviceManager->GetChildrenCount() == 0) {
            return LFX_ERROR_NODEVS;
        }

        try {
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                deviceManager->GetChildByIndex(i)->Reset();
            }
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Update() {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (deviceManager->GetChildrenCount() == 0) {
            return LFX_ERROR_NODEVS;
        }

        try {
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                deviceManager->GetChildByIndex(i)->Update();
            }
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_UpdateDefault() {
        // Not supported
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumDevices(unsigned int* const numDevices) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        unsigned int numberOfDevices = deviceManager->GetChildrenCount();
        if (numberOfDevices == 0) {
            return LFX_ERROR_NODEVS;
        }

        *numDevices = numberOfDevices;
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetDeviceDescription(const unsigned int devIndex, char* const devDesc, const unsigned int devDescSize, unsigned char* const devType) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        unsigned int numberOfDevices = deviceManager->GetChildrenCount();
        if (devIndex > numberOfDevices) {
            return LFX_ERROR_NODEVS;
        }

        auto device = deviceManager->GetChildByIndex(devIndex);
        string deviceName = wstring_to_string(device->GetDeviceName());
        if (deviceName.length() > devDescSize) {
            return LFX_ERROR_BUFFSIZE;
        }

        try {
            sprintf_s(devDesc, devDescSize, deviceName.c_str());
            *devType = DeviceTypeToLfxDeviceType(device->GetDeviceType());
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (devIndex > deviceManager->GetChildrenCount()) {
            return LFX_ERROR_NODEVS;
        }

        try {
            *numLights = deviceManager->GetChildByIndex(devIndex)->GetNumberOfLights();
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, char* const lightDesc, const unsigned int lightDescSize) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (devIndex > deviceManager->GetChildrenCount()) {
            return LFX_ERROR_NODEVS;
        }

        auto device = deviceManager->GetChildByIndex(devIndex);
        if (lightIndex > device->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        string lightName = wstring_to_string(device->GetLightData(lightIndex).Name);
        if (lightName.length() > lightDescSize) {
            return LFX_ERROR_BUFFSIZE;
        }

        try {
            sprintf_s(lightDesc, lightDescSize, lightName.c_str());
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (devIndex > deviceManager->GetChildrenCount()) {
            return LFX_ERROR_NODEVS;
        }

        auto device = deviceManager->GetChildByIndex(devIndex);
        if (lightIndex > device->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        try {
            *lightLoc = DeviceLightPositionToLfxPosition(device->GetLightData(lightIndex).Position);
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (devIndex > deviceManager->GetChildrenCount()) {
            return LFX_ERROR_NODEVS;
        }

        auto device = deviceManager->GetChildByIndex(devIndex);
        if (lightIndex > device->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        try {
            *lightCol = LightColorToLfxColor(device->GetCurrentLightAction().GetCurrentColor(lightIndex));
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const PLFX_COLOR lightCol) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (devIndex > deviceManager->GetChildrenCount()) {
            return LFX_ERROR_NODEVS;
        }

        auto device = deviceManager->GetChildByIndex(devIndex);
        if (lightIndex > device->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        LightAction lightAction = device->GetQueuedLightAction();
        if (lightAction.GetStartColor().size() == 0) {
            lightAction = LightAction(device->GetNumberOfLights());
        }

        try {
            lightAction.SetActionType(LightActionType::Instant);
            lightAction.SetStartColor(lightIndex, LfxColorToLightColor(*lightCol));
            device->QueueLightAction(lightAction);
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Light(const unsigned int locationMask, const unsigned int lightCol) {
        // Location mask not supported yet, so we set everything
        LightColor color = IntToLightColor(lightCol);

        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        try {
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                device->QueueLightAction(LightAction::NewInstant(device->GetNumberOfLights(), color));
            }
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (devIndex > deviceManager->GetChildrenCount()) {
            return LFX_ERROR_NODEVS;
        }

        auto device = deviceManager->GetChildByIndex(devIndex);
        if (lightIndex > device->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        try {
            LightAction lightAction;
            switch (actionType) {
            case LFX_ACTION_MORPH:
                lightAction = LightAction::NewMorph(device->GetNumberOfLights(), LightColor(), LfxColorToLightColor(*primaryCol), timing);
                break;

            case LFX_ACTION_PULSE:
                lightAction = LightAction::NewPulse(device->GetNumberOfLights(), LightColor(), LfxColorToLightColor(*primaryCol), timing, 1);
                break;

            default:
                lightAction = LightAction::NewInstant(device->GetNumberOfLights(), LfxColorToLightColor(*primaryCol));
                break;
            }
            lightAction.SetStartColor(device->GetCurrentLightAction().GetCurrentColor());
            deviceManager->GetChildByIndex(devIndex)->QueueLightAction(lightAction);
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol, const PLFX_COLOR secondaryCol) {
        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        if (devIndex > deviceManager->GetChildrenCount()) {
            return LFX_ERROR_NODEVS;
        }

        auto device = deviceManager->GetChildByIndex(devIndex);
        if (lightIndex > device->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        try {
            LightAction lightAction;
            switch (actionType) {
            case LFX_ACTION_MORPH:
                lightAction = LightAction::NewMorph(device->GetNumberOfLights(), LfxColorToLightColor(*primaryCol), LfxColorToLightColor(*secondaryCol), timing);
                break;

            case LFX_ACTION_PULSE:
                lightAction = LightAction::NewPulse(device->GetNumberOfLights(), LfxColorToLightColor(*primaryCol), LfxColorToLightColor(*secondaryCol), timing, 1);
                break;

            default:
                lightAction = LightAction::NewInstant(device->GetNumberOfLights(), LfxColorToLightColor(*secondaryCol));
                break;
            }
            deviceManager->GetChildByIndex(devIndex)->QueueLightAction(lightAction);
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColor(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol) {
        // Location mask not supported yet, so we set everything
        LFX_COLOR color = IntToLfxColor(primaryCol);

        LFX_RESULT result;
        auto deviceManager = lightFXExtender->GetDeviceManager();
        for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
            auto device = deviceManager->GetChildByIndex(i);
            for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                result = LFX_SetLightActionColor(i, j, actionType, &color);
                if (result != LFX_SUCCESS) {
                    return result;
                }
            }
        }
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColorEx(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol, const unsigned int secondaryCol) {
        // Location mask not supported yet, so we set everything
        LFX_COLOR color1 = IntToLfxColor(primaryCol);
        LFX_COLOR color2 = IntToLfxColor(secondaryCol);

        LFX_RESULT result;
        auto deviceManager = lightFXExtender->GetDeviceManager();
        for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
            auto device = deviceManager->GetChildByIndex(i);
            for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                result = LFX_SetLightActionColorEx(i, j, actionType, &color1, &color2);
                if (result != LFX_SUCCESS) {
                    return result;
                }
            }
        }
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetTiming(const int newTiming) {
        timing = newTiming;
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetVersion(char* const version, const unsigned int versionSize) {
        sprintf_s(version, versionSize, "2.2.0.0");
        return LFX_SUCCESS;
    }

#ifdef __cplusplus
}
#endif

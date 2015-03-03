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
            switch (device->GetDeviceType()) {
            case DeviceType::DeviceUnknown:
                *devType = 0x00;
                break;
            case DeviceType::DeviceDisplay:
                *devType = 0x04;
                break;
            case DeviceType::DeviceKeyboard:
                *devType = 0x06;
                break;
            case DeviceType::DeviceOther:
                *devType = 0xFF;
                break;
            }
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
            auto lightData = device->GetLightData(lightIndex);
            *lightLoc = LFX_POSITION{ lightData.Position.x, lightData.Position.y, lightData.Position.z };
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
            auto lightColor = device->GetCurrentLightAction().GetCurrentColor(lightIndex);
            *lightCol = LFX_COLOR{ lightColor.red, lightColor.green, lightColor.blue, lightColor.brightness };
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
            lightAction.SetStartColor(lightIndex, LightColor(lightCol->red, lightCol->green, lightCol->blue, lightCol->brightness));
            device->QueueLightAction(lightAction);
        } catch (...) {
            return LFX_FAILURE;
        }
        
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Light(const unsigned int locationMask, const unsigned int lightCol) {
        // Location mask not supported yet, so we set everything
        LightColor color;
        color.blue = (lightCol >> 24) & 0xFF;
        color.green = (lightCol >> 16) & 0xFF;
        color.red = (lightCol >> 8) & 0xFF;
        color.brightness = lightCol & 0xFF;

        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        try {
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                device->QueueLightAction(LightAction(device->GetNumberOfLights(), color));
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
            lightAction.SetEndColor(LightColor(primaryCol->red, primaryCol->green, primaryCol->blue, primaryCol->brightness));
            lightAction.SetActionTime(timing);
            switch (actionType) {
            case LFX_ACTION_MORPH:
                lightAction.SetActionType(LightActionType::Morph);
                break;

            case LFX_ACTION_PULSE:
                lightAction.SetActionType(LightActionType::Pulse);

            default:
                lightAction.SetActionType(LightActionType::Instant);
                lightAction.SetStartColor(lightAction.GetEndColor());
            }

            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                if (lightAction.GetActionType() != LightActionType::Instant) {
                    for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                        lightAction.SetStartColor(j, device->GetCurrentLightAction().GetCurrentColor(j));
                    }
                }
                device->QueueLightAction(lightAction);
            }
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
            lightAction.SetStartColor(LightColor(primaryCol->red, primaryCol->green, primaryCol->blue, primaryCol->brightness));
            lightAction.SetEndColor(LightColor(secondaryCol->red, secondaryCol->green, secondaryCol->blue, secondaryCol->brightness));
            lightAction.SetActionTime(timing);
            switch (actionType) {
            case LFX_ACTION_MORPH:
                lightAction.SetActionType(LightActionType::Morph);
                break;

            case LFX_ACTION_PULSE:
                lightAction.SetActionType(LightActionType::Pulse);
                break;

            default:
                lightAction.SetActionType(LightActionType::Instant);
                lightAction.SetStartColor(lightAction.GetEndColor());
                break;
            }

            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                deviceManager->GetChildByIndex(i)->QueueLightAction(lightAction);
            }
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColor(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol) {
        // Location mask not supported yet, so we set everything
        LightColor color;
        color.blue = (primaryCol >> 24) & 0xFF;
        color.green = (primaryCol >> 16) & 0xFF;
        color.red = (primaryCol >> 8) & 0xFF;
        color.brightness = primaryCol & 0xFF;

        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        try {
            LightAction lightAction;
            lightAction.SetEndColor(color);
            lightAction.SetActionTime(timing);
            switch (actionType) {
            case LFX_ACTION_MORPH:
                lightAction.SetActionType(LightActionType::Morph);
                break;

            case LFX_ACTION_PULSE:
                lightAction.SetActionType(LightActionType::Pulse);

            default:
                lightAction.SetActionType(LightActionType::Instant);
                lightAction.SetStartColor(lightAction.GetEndColor());
            }

            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                if (lightAction.GetActionType() != LightActionType::Instant) {
                    for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                        lightAction.SetStartColor(j, device->GetCurrentLightAction().GetCurrentColor(j));
                    }
                }
                device->QueueLightAction(lightAction);
            }
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColorEx(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol, const unsigned int secondaryCol) {
        // Location mask not supported yet, so we set everything
        LightColor color1;
        color1.blue = (primaryCol >> 24) & 0xFF;
        color1.green = (primaryCol >> 16) & 0xFF;
        color1.red = (primaryCol >> 8) & 0xFF;
        color1.brightness = primaryCol & 0xFF;
        LightColor color2;
        color2.blue = (secondaryCol >> 24) & 0xFF;
        color2.green = (secondaryCol >> 16) & 0xFF;
        color2.red = (secondaryCol >> 8) & 0xFF;
        color2.brightness = secondaryCol & 0xFF;

        if (!lightFXExtender->IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        auto deviceManager = lightFXExtender->GetDeviceManager();
        try {
            LightAction lightAction;
            lightAction.SetStartColor(color1);
            lightAction.SetEndColor(color2);
            lightAction.SetActionTime(timing);
            switch (actionType) {
            case LFX_ACTION_MORPH:
                lightAction.SetActionType(LightActionType::Morph);
                break;

            case LFX_ACTION_PULSE:
                lightAction.SetActionType(LightActionType::Pulse);

            default:
                lightAction.SetActionType(LightActionType::Instant);
                lightAction.SetStartColor(lightAction.GetEndColor());
            }

            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                deviceManager->GetChildByIndex(i)->QueueLightAction(lightAction);
            }
        } catch (...) {
            return LFX_FAILURE;
        }

        return LFX_SUCCESS;
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

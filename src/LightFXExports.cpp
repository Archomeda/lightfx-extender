#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

// Standard includes
#include <limits>
#include <memory>
#include <string>

// 3rd party includes
#include "LFX2.h"

// Project includes
#include "lightFXExtender.h"
#include "Managers/DeviceManager.h"
#include "Managers/UpdateManager.h"
#include "Utils/Log.h"
#include "Utils/String.h"


using namespace std;
using namespace lightfx;
using namespace lightfx::devices;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;


#pragma region Converters

LFX_POSITION DeviceLightPositionToLfxPosition(const DeviceLightPosition position) {
    return LFX_POSITION{ position.x, position.y, position.z };
}

LFX_COLOR LightColorToLfxColor(const LightColor color) {
    return LFX_COLOR{ color.red, color.green, color.blue, color.brightness };
}

LFX_COLOR IntToLfxColor(const unsigned int color) {
    LFX_COLOR c;
    c.brightness = (color >> 24) & 0xFF;
    c.red = (color >> 16) & 0xFF;
    c.green = (color >> 8) & 0xFF;
    c.blue = color & 0xFF;
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
        try {

            lightFXExtender->Initialize();
            lightFXExtender->Start();
            return lightFXExtender->GetDeviceManager()->GetChildrenCount() > 0 ? LFX_SUCCESS : LFX_ERROR_NODEVS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_Initialize: " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Release() {
        try {

            lightFXExtender->Stop();
            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_Release: " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Reset() {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (deviceManager->GetChildrenCount() == 0) {
                return LFX_ERROR_NODEVS;
            }

            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                deviceManager->GetChildByIndex(i)->Reset();
            }

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_Reset: " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Update() {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (deviceManager->GetChildrenCount() == 0) {
                return LFX_ERROR_NODEVS;
            }

            deviceManager->UpdateDeviceLights();
            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_Update: " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_UpdateDefault() {
        // Not supported
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumDevices(unsigned int* const numDevices) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            size_t numberOfDevices = deviceManager->GetChildrenCount();
            if (numberOfDevices == 0) {
                return LFX_ERROR_NODEVS;
            } else if (numberOfDevices > numeric_limits<unsigned int>::max()) {
                return LFX_FAILURE;
            }

            *numDevices = static_cast<unsigned int>(numberOfDevices);
            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_GetNumDevices: " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetDeviceDescription(const unsigned int devIndex, char* const devDesc, const unsigned int devDescSize, unsigned char* const devType) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            auto device = deviceManager->GetChildByIndex(devIndex);
            string deviceName = wstring_to_string(device->GetDeviceName());
            if (deviceName.length() > devDescSize) {
                return LFX_ERROR_BUFFSIZE;
            }

            sprintf_s(devDesc, devDescSize, deviceName.c_str());
            *devType = device->GetDeviceType();

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_GetDeviceDescription (devIndex=" + to_wstring(devIndex) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            size_t numberOfLights = deviceManager->GetChildByIndex(devIndex)->GetNumberOfLights();
            if (numberOfLights > numeric_limits<unsigned int>::max()) {
                return LFX_FAILURE;
            }
            *numLights = static_cast<unsigned int>(numberOfLights);

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_GetNumLights (devIndex=" + to_wstring(devIndex) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, char* const lightDesc, const unsigned int lightDescSize) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            auto device = deviceManager->GetChildByIndex(devIndex);
            if (lightIndex >= device->GetNumberOfLights()) {
                return LFX_ERROR_NOLIGHTS;
            }

            string lightName = wstring_to_string(device->GetLightData(lightIndex).Name);
            if (lightName.length() > lightDescSize) {
                return LFX_ERROR_BUFFSIZE;
            }

            sprintf_s(lightDesc, lightDescSize, lightName.c_str());

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_GetLightDescription (devIndex=" + to_wstring(devIndex) + L", lightIndex=" + to_wstring(lightIndex) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            auto device = deviceManager->GetChildByIndex(devIndex);
            if (lightIndex >= device->GetNumberOfLights()) {
                return LFX_ERROR_NOLIGHTS;
            }

            *lightLoc = DeviceLightPositionToLfxPosition(device->GetLightData(lightIndex).Position);

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_GetLightLocation (devIndex=" + to_wstring(devIndex) + L", lightIndex=" + to_wstring(lightIndex) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            auto device = deviceManager->GetChildByIndex(devIndex);
            if (lightIndex >= device->GetNumberOfLights()) {
                return LFX_ERROR_NOLIGHTS;
            }

            *lightCol = LightColorToLfxColor(device->GetLightColor(lightIndex));

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_GetLightColor (devIndex=" + to_wstring(devIndex) + L", lightIndex=" + to_wstring(lightIndex) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const PLFX_COLOR lightCol) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            auto device = deviceManager->GetChildByIndex(devIndex);
            if (lightIndex >= device->GetNumberOfLights()) {
                return LFX_ERROR_NOLIGHTS;
            }

            Timeline timeline = device->GetQueuedTimeline();
            LightColor color = LfxColorToLightColor(*lightCol);

            timeline.SetTimeline(lightIndex, LightTimeline::NewInstant(color));
            device->QueueTimeline(timeline);

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_GetLightColor (devIndex=" + to_wstring(devIndex) + L", lightIndex=" + to_wstring(lightIndex) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Light(const unsigned int locationMask, const unsigned int lightCol) {
        try {

            // Location mask not supported yet, so we set everything
            LightColor color = IntToLightColor(lightCol);

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                device->QueueTimeline(Timeline(device->GetNumberOfLights(), LightTimeline::NewInstant(color)));
            }

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_Light (locationMask=" + to_wstring(locationMask) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            auto device = deviceManager->GetChildByIndex(devIndex);
            if (lightIndex >= device->GetNumberOfLights()) {
                return LFX_ERROR_NOLIGHTS;
            }

            Timeline timeline = device->GetQueuedTimeline();
            LFX_COLOR startColor = LightColorToLfxColor(device->GetLightColor(lightIndex));
            return LFX_SetLightActionColorEx(devIndex, lightIndex, actionType, &startColor, primaryCol);

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_SetLightActionColor (devIndex=" + to_wstring(devIndex) + L", lightIndex=" + to_wstring(lightIndex) + L", actionType=" + to_wstring(actionType) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol, const PLFX_COLOR secondaryCol) {
        try {

            if (!lightFXExtender->IsInitialized()) {
                return LFX_ERROR_NOINIT;
            }

            auto deviceManager = lightFXExtender->GetDeviceManager();
            if (devIndex >= deviceManager->GetChildrenCount()) {
                return LFX_ERROR_NODEVS;
            }

            auto device = deviceManager->GetChildByIndex(devIndex);
            if (lightIndex >= device->GetNumberOfLights()) {
                return LFX_ERROR_NOLIGHTS;
            }

            Timeline timeline = device->GetQueuedTimeline();
            LightColor startColor = LfxColorToLightColor(*primaryCol);
            LightColor endColor = LfxColorToLightColor(*secondaryCol);
            switch (actionType) {
            case LFX_ACTION_MORPH:
                timeline.SetTimeline(lightIndex, LightTimeline::NewMorph(startColor, endColor, timing));
                break;

            case LFX_ACTION_PULSE:
                timeline.SetTimeline(lightIndex, LightTimeline::NewPulse(startColor, endColor, timing, 100)); //TODO: Make this infinite instead of a hardcoded amount
                break;

            default:
                timeline.SetTimeline(lightIndex, LightTimeline::NewInstant(endColor));
                break;
            }
            deviceManager->GetChildByIndex(devIndex)->QueueTimeline(timeline);

            return LFX_SUCCESS;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_SetLightActionColorEx (devIndex=" + to_wstring(devIndex) + L", lightIndex=" + to_wstring(lightIndex) + L", actionType=" + to_wstring(actionType) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColor(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol) {
        try {

            // Location mask not supported yet, so we set everything
            LFX_COLOR color = IntToLfxColor(primaryCol);

            LFX_RESULT result;
            auto deviceManager = lightFXExtender->GetDeviceManager();
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                if (i > numeric_limits<unsigned int>::max()) {
                    return LFX_FAILURE;
                }

                auto device = deviceManager->GetChildByIndex(i);
                for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                    if (j > numeric_limits<unsigned int>::max()) {
                        return LFX_FAILURE;
                    }

                    result = LFX_SetLightActionColor(static_cast<unsigned int>(i), static_cast<unsigned int>(j), actionType, &color);
                    if (result != LFX_SUCCESS) {
                        return result;
                    }
                }
            }
            return result;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_ActionColor (locationMask=" + to_wstring(locationMask) + L", actionType=" + to_wstring(actionType) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColorEx(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol, const unsigned int secondaryCol) {
        try {

            // Location mask not supported yet, so we set everything
            LFX_COLOR color1 = IntToLfxColor(primaryCol);
            LFX_COLOR color2 = IntToLfxColor(secondaryCol);

            LFX_RESULT result;
            auto deviceManager = lightFXExtender->GetDeviceManager();
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                if (i > numeric_limits<unsigned int>::max()) {
                    return LFX_FAILURE;
                }

                auto device = deviceManager->GetChildByIndex(i);
                for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                    if (j > numeric_limits<unsigned int>::max()) {
                        return LFX_FAILURE;
                    }

                    result = LFX_SetLightActionColorEx(static_cast<unsigned int>(i), static_cast<unsigned int>(j), actionType, &color1, &color2);
                    if (result != LFX_SUCCESS) {
                        return result;
                    }
                }
            }
            return result;

        } catch (const exception& ex) {
            LOG_ERROR(L"Exception in LFX_ActionColorEx (locationMask=" + to_wstring(locationMask) + L", actionType=" + to_wstring(actionType) + L"): " + string_to_wstring(ex.what()));
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetTiming(const int newTiming) {
        timing = newTiming;
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetVersion(char* const version, const unsigned int versionSize) {
        sprintf_s(version, versionSize, "2.2.0.0");
        return LFX_SUCCESS;
    }

    // Custom export to help check if we are loading ourselves
    FN_DECLSPEC LFX_RESULT STDCALL LFXE_GetVersion(char* const version, const unsigned int versionSize) {
        Version ver = lightFXExtender->GetUpdateManager()->GetCurrentVersion();
        string verString = wstring_to_string(ver.ToString());
        if (verString.length() > versionSize) {
            return LFX_ERROR_BUFFSIZE;
        }

        sprintf_s(version, versionSize, verString.c_str());
        return LFX_SUCCESS;
    }

#ifdef __cplusplus
}
#endif

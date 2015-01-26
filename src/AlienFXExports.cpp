// Standard includes
#include <string>
#include <locale>
#include <codecvt>

// 3rd party includes
#include "LFX2.h"

// Project includes
#include "Utils/String.h"
#include "LightsManager.h"


using namespace std;
using namespace lightfx;
using namespace lightfx::utils;

#ifdef __cplusplus
extern "C" {
#endif

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Initialize() {
        return LightsManager::Instance().Initialize() > 0 ? LFX_SUCCESS : LFX_ERROR_NODEVS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Release() {
        LightsManager::Instance().Release();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Reset() {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (!LightsManager::Instance().HasDevices()) {
            return LFX_ERROR_NODEVS;
        }

        LightsManager::Instance().Reset();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Update() {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (!LightsManager::Instance().HasDevices()) {
            return LFX_ERROR_NODEVS;
        }

        LightsManager::Instance().Update();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_UpdateDefault() {
        // Not supported
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumDevices(unsigned int* const numDevices) {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (!LightsManager::Instance().HasDevices()) {
            return LFX_ERROR_NODEVS;
        }

        *numDevices = LightsManager::Instance().GetNumberOfDevices();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetDeviceDescription(const unsigned int devIndex, char* const devDesc, const unsigned int devDescSize, unsigned char* const devType) {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > LightsManager::Instance().GetNumberOfDevices()) {
            return LFX_ERROR_NODEVS;
        }

        string deviceName = wstring_to_string(LightsManager::Instance().GetDevice(devIndex)->GetDeviceName());
        if (deviceName.length() > devDescSize) {
            return LFX_ERROR_BUFFSIZE;
        }

        *devDesc = *deviceName.c_str();
        *devType = LightsManager::Instance().GetDevice(devIndex)->GetDeviceType();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > LightsManager::Instance().GetNumberOfDevices()) {
            return LFX_ERROR_NODEVS;
        }

        *numLights = LightsManager::Instance().GetDevice(devIndex)->GetNumberOfLights();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, char* const lightDesc, const unsigned int lightDescSize) {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > LightsManager::Instance().GetNumberOfDevices()) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > LightsManager::Instance().GetDevice(devIndex)->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        string lightName = wstring_to_string(LightsManager::Instance().GetDevice(devIndex)->GetLight(lightIndex).Name);
        if (lightName.length() > lightDescSize) {
            return LFX_ERROR_BUFFSIZE;
        }

        *lightDesc = *lightName.c_str();
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > LightsManager::Instance().GetNumberOfDevices()) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > LightsManager::Instance().GetDevice(devIndex)->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        *lightLoc = LightsManager::Instance().GetDevice(devIndex)->GetLight(lightIndex).Position;
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > LightsManager::Instance().GetNumberOfDevices()) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > LightsManager::Instance().GetDevice(devIndex)->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }

        *lightCol = LightsManager::Instance().GetDevice(devIndex)->GetPrimaryColorForLight(lightIndex);
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const PLFX_COLOR lightCol) {
        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > LightsManager::Instance().GetNumberOfDevices()) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > LightsManager::Instance().GetDevice(devIndex)->GetNumberOfLights()) {
            return LFX_ERROR_NOLIGHTS;
        }
        
        LightsManager::Instance().GetDevice(devIndex)->SetPrimaryColorForLight(lightIndex, *lightCol);
        return LFX_SUCCESS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Light(const unsigned int locationMask, const unsigned int lightCol) {
        LFX_COLOR col;
        col.blue = (lightCol >> 24) & 0xFF;
        col.green = (lightCol >> 16) & 0xFF;
        col.red = (lightCol >> 8) & 0xFF;
        col.brightness = lightCol & 0xFF;
        LightLocationMask mask = static_cast<LightLocationMask>(locationMask);

        if (!LightsManager::Instance().IsInitialized()) {
            return LFX_ERROR_NOINIT;
        }

        return LightsManager::Instance().SetColorForLocation(mask, col) > 0 ? LFX_SUCCESS : LFX_ERROR_NOLIGHTS;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol) {
        // Not yet implemented
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol, const PLFX_COLOR secondaryCol) {
        // Not yet implemented
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColor(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol) {
        LFX_COLOR col;
        col.blue = (primaryCol >> 24) & 0xFF;
        col.green = (primaryCol >> 16) & 0xFF;
        col.red = (primaryCol >> 8) & 0xFF;
        col.brightness = primaryCol & 0xFF;

        // Not yet implemented
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColorEx(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol, const unsigned int secondaryCol) {
        LFX_COLOR pcol, scol;
        pcol.blue = (primaryCol >> 24) & 0xFF;
        pcol.green = (primaryCol >> 16) & 0xFF;
        pcol.red = (primaryCol >> 8) & 0xFF;
        pcol.brightness = primaryCol & 0xFF;
        scol.blue = (secondaryCol >> 24) & 0xFF;
        scol.green = (secondaryCol >> 16) & 0xFF;
        scol.red = (secondaryCol >> 8) & 0xFF;
        scol.brightness = secondaryCol & 0xFF;

        // Not yet implemented
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetTiming(const int newTiming) {
        // Not yet implemented
        return LFX_FAILURE;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetVersion(char* const version, const unsigned int versionSize) {
        *version = *"2.2.0.0";
        return LFX_SUCCESS;
    }

#ifdef __cplusplus
}
#endif

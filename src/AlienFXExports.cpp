#include <string>
#include "LFX2.h"

#include "DeviceManager.h"

using namespace std;
using namespace lightfx;

DeviceManager deviceManager = DeviceManager();

#ifdef __cplusplus
extern "C" {
#endif

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Initialize() {
        LFX_RESULT result = deviceManager.Initialize() ? LFX_SUCCESS : LFX_FAILURE;
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Release() {
        LFX_RESULT result = deviceManager.Dispose() ? LFX_SUCCESS : LFX_FAILURE;
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Reset() {
        LFX_RESULT result = deviceManager.GetLightFXManager()->Reset();
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Update() {
        LFX_RESULT result = deviceManager.GetLightFXManager()->Update();
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_UpdateDefault() {
        LFX_RESULT result = deviceManager.GetLightFXManager()->UpdateDefault();
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumDevices(unsigned int* const numDevices) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetNumDevices(*numDevices);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetDeviceDescription(const unsigned int devIndex, char* const devDesc, const unsigned int devDescSize, unsigned char* const devType) {
        string deviceName;
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetDeviceDescription(devIndex, deviceName, devDescSize, *devType);
        *devDesc = *deviceName.c_str();
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetNumLights(devIndex, *numLights);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, char* const lightDesc, const unsigned int devDescSize) {
        string lightName;
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetLightDescription(devIndex, lightIndex, lightName, devDescSize);
        *lightDesc = *lightName.c_str();
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetLightLocation(devIndex, lightIndex, *lightLoc);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetLightColor(devIndex, lightIndex, *lightCol);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const PLFX_COLOR lightCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetLightColor(devIndex, lightIndex, *lightCol);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Light(const unsigned int locationMask, const unsigned int lightCol) {
        LFX_COLOR col;
        col.blue = (lightCol >> 24) & 0xFF;
        col.green = (lightCol >> 16) & 0xFF;
        col.red = (lightCol >> 8) & 0xFF;
        col.brightness = lightCol & 0xFF;
        LFX_RESULT result = deviceManager.GetLightFXManager()->Light(locationMask, col);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetLightActionColor(devIndex, lightIndex, actionType, *primaryCol);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol, const PLFX_COLOR secondaryCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetLightActionColor(devIndex, lightIndex, actionType, *primaryCol, *secondaryCol);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColor(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol) {
        LFX_COLOR col;
        col.blue = (primaryCol >> 24) & 0xFF;
        col.green = (primaryCol >> 16) & 0xFF;
        col.red = (primaryCol >> 8) & 0xFF;
        col.brightness = primaryCol & 0xFF;
        LFX_RESULT result = deviceManager.GetLightFXManager()->ActionColor(locationMask, actionType, col);
        return result;
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
        LFX_RESULT result = deviceManager.GetLightFXManager()->ActionColor(locationMask, actionType, pcol, scol);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetTiming(const int newTiming) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetTiming(newTiming);
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetVersion(char* const version, const unsigned int versionSize) {
        string ver;
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetVersion(ver, versionSize);
        *version = *ver.c_str();
        return result;
    }

#ifdef __cplusplus
}
#endif

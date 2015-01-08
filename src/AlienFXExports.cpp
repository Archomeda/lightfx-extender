#include <string>
#include "LFX2.h"

#include "DeviceManager.h"
#include "Log.h"

using namespace std;
using namespace lightfx;

DeviceManager deviceManager = DeviceManager();

#ifdef __cplusplus
extern "C" {
#endif

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Initialize() {
        LFX_RESULT result = deviceManager.Initialize() ? LFX_SUCCESS : LFX_FAILURE;
        Log("LFX_Initialize(): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Release() {
        LFX_RESULT result = deviceManager.Dispose() ? LFX_SUCCESS : LFX_FAILURE;
        Log("LFX_Release(): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Reset() {
        LFX_RESULT result = deviceManager.GetLightFXManager()->Reset();
        Log("LFX_Reset(): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Update() {
        LFX_RESULT result = deviceManager.GetLightFXManager()->Update();
        Log("LFX_Update(): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_UpdateDefault() {
        LFX_RESULT result = deviceManager.GetLightFXManager()->UpdateDefault();
        Log("LFX_UpdateDefault(): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumDevices(unsigned int* const numDevices) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetNumDevices(*numDevices);
        Log("LFX_GetNumDevices(<" + to_string(*numDevices) + ">): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetDeviceDescription(const unsigned int devIndex, char* const devDesc, const unsigned int devDescSize, unsigned char* const devType) {
        string deviceName;
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetDeviceDescription(devIndex, deviceName, devDescSize, *devType);
        *devDesc = *deviceName.c_str();
        Log("LFX_GetDeviceDescription(" + to_string(devIndex) + ", <" + deviceName + ">, " + to_string(devDescSize) + ", <" + to_string(*devType) + ">): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetNumLights(devIndex, *numLights);
        Log("LFX_GetNumLights(" + to_string(devIndex) + ", <" + to_string(*numLights) + ">): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, char* const lightDesc, const unsigned int devDescSize) {
        string lightName;
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetLightDescription(devIndex, lightIndex, lightName, devDescSize);
        *lightDesc = *lightName.c_str();
        Log("LFX_GetLightDescription(" + to_string(devIndex) + ", " + to_string(lightIndex) + ", <" + lightName + ">, " + to_string(devDescSize) + "): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetLightLocation(devIndex, lightIndex, *lightLoc);
        Log("LFX_GetLightLocation(" + to_string(devIndex) + ", " + to_string(lightIndex) + ", <(" + to_string(lightLoc->x) + ", " + to_string(lightLoc->y) + ", " + to_string(lightLoc->z) + ")>): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetLightColor(devIndex, lightIndex, *lightCol);
        Log("LFX_GetLightColor(" + to_string(devIndex) + ", " + to_string(lightIndex) + ", <(" + to_string(lightCol->red) + ", " + to_string(lightCol->blue) + ", " + to_string(lightCol->green) + ", " + to_string(lightCol->brightness) + ")>): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const PLFX_COLOR lightCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetLightColor(devIndex, lightIndex, *lightCol);
        Log("LFX_SetLightColor(" + to_string(devIndex) + ", " + to_string(lightIndex) + ", (" + to_string(lightCol->red) + ", " + to_string(lightCol->blue) + ", " + to_string(lightCol->green) + ", " + to_string(lightCol->brightness) + ")): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Light(const unsigned int locationMask, const unsigned int lightCol) {
        LFX_COLOR col;
        col.blue = (lightCol >> 24) & 0xFF;
        col.green = (lightCol >> 16) & 0xFF;
        col.red = (lightCol >> 8) & 0xFF;
        col.brightness = lightCol & 0xFF;
        LFX_RESULT result = deviceManager.GetLightFXManager()->Light(locationMask, col);
        Log("LFX_Light(" + to_string(locationMask) + ", (" + to_string(col.red) + ", " + to_string(col.blue) + ", " + to_string(col.green) + ", " + to_string(col.brightness) + ")): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetLightActionColor(devIndex, lightIndex, actionType, *primaryCol);
        Log("LFX_SetLightActionColor(" + to_string(devIndex) + ", " + to_string(lightIndex) + ", " + to_string(actionType) + ", (" + to_string(primaryCol->red) + ", " + to_string(primaryCol->blue) + ", " + to_string(primaryCol->green) + ", " + to_string(primaryCol->brightness) + ")): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol, const PLFX_COLOR secondaryCol) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetLightActionColor(devIndex, lightIndex, actionType, *primaryCol, *secondaryCol);
        Log("LFX_SetLightActionColorEx(" + to_string(devIndex) + ", " + to_string(lightIndex) + ", " + to_string(actionType) + ", (" + to_string(primaryCol->red) + ", " + to_string(primaryCol->blue) + ", " + to_string(primaryCol->green) + ", " + to_string(primaryCol->brightness) + "), (" + to_string(secondaryCol->red) + ", " + to_string(secondaryCol->blue) + ", " + to_string(secondaryCol->green) + ", " + to_string(secondaryCol->brightness) + ")): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColor(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol) {
        LFX_COLOR col;
        col.blue = (primaryCol >> 24) & 0xFF;
        col.green = (primaryCol >> 16) & 0xFF;
        col.red = (primaryCol >> 8) & 0xFF;
        col.brightness = primaryCol & 0xFF;
        LFX_RESULT result = deviceManager.GetLightFXManager()->ActionColor(locationMask, actionType, col);
        Log("LFX_ActionColor(" + to_string(locationMask) + ", " + to_string(actionType) + ", (" + to_string(col.red) + ", " + to_string(col.blue) + ", " + to_string(col.green) + ", " + to_string(col.brightness) + ")): " + to_string(result));
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
        Log("LFX_ActionColorEx(" + to_string(locationMask) + ", " + to_string(actionType) + ", (" + to_string(pcol.red) + ", " + to_string(pcol.blue) + ", " + to_string(pcol.green) + ", " + to_string(pcol.brightness) + "), (" + to_string(scol.red) + ", " + to_string(scol.blue) + ", " + to_string(scol.green) + ", " + to_string(scol.brightness) + ")): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetTiming(const int newTiming) {
        LFX_RESULT result = deviceManager.GetLightFXManager()->SetTiming(newTiming);
        Log("LFX_SetTiming(" + to_string(newTiming) + "): " + to_string(result));
        return result;
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetVersion(char* const version, const unsigned int versionSize) {
        string ver;
        LFX_RESULT result = deviceManager.GetLightFXManager()->GetVersion(ver, versionSize);
        *version = *ver.c_str();
        Log("LFX_GetVersion(<" + ver + ">, " + to_string(versionSize) + "): " + to_string(result));
        return result;
    }

#ifdef __cplusplus
}
#endif

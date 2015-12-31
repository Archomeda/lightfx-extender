// This library is a LightFX proxy and is used to forward all LightFX calls to LightFX Extender.
// The resulting file "LightFX.dll" is placed in the game's directory and copies the original LightFX behavior.
// This has the advantage that the file "LightFX.dll" is small in size and can be copied to multiple games,
// while having a main DLL file that is maintained in a central directory and handles all the logic for lights.

#define LFXE_EXPORTS

#ifdef LFXE_EXPORTS
#define LFXE_API _declspec(dllexport)
#else
#define LFXE_API _declspec(dllimport)
#endif


#ifdef _WIN64
#define LFXE_DLL_NAME L"LFXE64.dll"
#else
#define LFXE_DLL_NAME L"LFXE.dll"
#endif


#include "Windows.h"
#include <ShlObj.h>

#include <string>

#include "LFX2.h"


typedef bool(*LFXEGETVERSION)(char* const, const unsigned int);

#define LFXE_DLL_GETVERSION "LFXE_GetVersion"

LFX2INITIALIZE Forward_LFX_Initialize;
LFX2RELEASE Forward_LFX_Release;
LFX2RESET Forward_LFX_Reset;
LFX2UPDATE Forward_LFX_Update;
LFX2UPDATEDEFAULT Forward_LFX_UpdateDefault;
LFX2GETNUMDEVICES Forward_LFX_GetNumDevices;
LFX2GETDEVDESC Forward_LFX_GetDeviceDescription;
LFX2GETNUMLIGHTS Forward_LFX_GetNumLights;
LFX2GETLIGHTDESC Forward_LFX_GetLightDescription;
LFX2GETLIGHTLOC Forward_LFX_GetLightLocation;
LFX2GETLIGHTCOL Forward_LFX_GetLightColor;
LFX2SETLIGHTCOL Forward_LFX_SetLightColor;
LFX2LIGHT Forward_LFX_Light;
LFX2SETLIGHTACTIONCOLOR Forward_LFX_SetLightActionColor;
LFX2SETLIGHTACTIONCOLOREX Forward_LFX_SetLightActionColorEx;
LFX2ACTIONCOLOR Forward_LFX_ActionColor;
LFX2ACTIONCOLOREX Forward_LFX_ActionColorEx;
LFX2SETTIMING Forward_LFX_SetTiming;
LFX2GETVERSION Forward_LFX_GetVersion;
LFXEGETVERSION Forward_LFXE_GetVersion;

HINSTANCE hInstanceForward = NULL;


using namespace std;


#ifdef __cplusplus
extern "C" {
#endif

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Initialize() {
        // Our entry point
        if (hInstanceForward) {
            // We are already initialized
            return LFX_SUCCESS;
        }

        // We assume that our main library is located inside "%APPDATA%\LightFX Extender"
        wstring path;
        wchar_t* appDataFolder = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataFolder))) {
            path = wstring(appDataFolder) + L"\\LightFX Extender\\" + LFXE_DLL_NAME;
            CoTaskMemFree(appDataFolder);
        } else {
            CoTaskMemFree(appDataFolder);
            return LFX_FAILURE;
        }

        // Load our library
        hInstanceForward = LoadLibraryW(path.c_str());
        if (!hInstanceForward) {
            return LFX_FAILURE;
        }
        Forward_LFX_Initialize = (LFX2INITIALIZE)GetProcAddress(hInstanceForward, LFX_DLL_INITIALIZE);
        Forward_LFX_Release = (LFX2RELEASE)GetProcAddress(hInstanceForward, LFX_DLL_RELEASE);
        Forward_LFX_Reset = (LFX2RESET)GetProcAddress(hInstanceForward, LFX_DLL_RESET);
        Forward_LFX_Update = (LFX2UPDATE)GetProcAddress(hInstanceForward, LFX_DLL_UPDATE);
        Forward_LFX_UpdateDefault = (LFX2UPDATEDEFAULT)GetProcAddress(hInstanceForward, LFX_DLL_UPDATEDEFAULT);
        Forward_LFX_GetNumDevices = (LFX2GETNUMDEVICES)GetProcAddress(hInstanceForward, LFX_DLL_GETNUMDEVICES);
        Forward_LFX_GetDeviceDescription = (LFX2GETDEVDESC)GetProcAddress(hInstanceForward, LFX_DLL_GETDEVDESC);
        Forward_LFX_GetNumLights = (LFX2GETNUMLIGHTS)GetProcAddress(hInstanceForward, LFX_DLL_GETNUMLIGHTS);
        Forward_LFX_GetLightDescription = (LFX2GETLIGHTDESC)GetProcAddress(hInstanceForward, LFX_DLL_GETLIGHTDESC);
        Forward_LFX_GetLightLocation = (LFX2GETLIGHTLOC)GetProcAddress(hInstanceForward, LFX_DLL_GETLIGHTLOC);
        Forward_LFX_GetLightColor = (LFX2GETLIGHTCOL)GetProcAddress(hInstanceForward, LFX_DLL_GETLIGHTCOL);
        Forward_LFX_SetLightColor = (LFX2SETLIGHTCOL)GetProcAddress(hInstanceForward, LFX_DLL_SETLIGHTCOL);
        Forward_LFX_Light = (LFX2LIGHT)GetProcAddress(hInstanceForward, LFX_DLL_LIGHT);
        Forward_LFX_SetLightActionColor = (LFX2SETLIGHTACTIONCOLOR)GetProcAddress(hInstanceForward, LFX_DLL_SETLIGHTACTIONCOLOR);
        Forward_LFX_SetLightActionColorEx = (LFX2SETLIGHTACTIONCOLOREX)GetProcAddress(hInstanceForward, LFX_DLL_SETLIGHTACTIONCOLOREX);
        Forward_LFX_ActionColor = (LFX2ACTIONCOLOR)GetProcAddress(hInstanceForward, LFX_DLL_ACTIONCOLOR);
        Forward_LFX_ActionColorEx = (LFX2ACTIONCOLOREX)GetProcAddress(hInstanceForward, LFX_DLL_ACTIONCOLOREX);
        Forward_LFX_SetTiming = (LFX2SETTIMING)GetProcAddress(hInstanceForward, LFX_DLL_SETTIMING);
        Forward_LFX_GetVersion = (LFX2GETVERSION)GetProcAddress(hInstanceForward, LFX_DLL_GETVERSION);
        Forward_LFXE_GetVersion = (LFXEGETVERSION)GetProcAddress(hInstanceForward, LFXE_DLL_GETVERSION);

        // Proceed loading externally and return afterwards
        return Forward_LFX_Initialize();
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Release() {
        // Our exit point

        if (!hInstanceForward) {
            // We are already unloaded
            return LFX_SUCCESS;
        }

        // Unload our library first
        LFX_RESULT resultRelease = Forward_LFX_Release();

        if (resultRelease != LFX_SUCCESS) {
            return resultRelease;
        }

        // Proceed unloading
        bool result = FreeLibrary(hInstanceForward) == TRUE;
        if (result) {
            hInstanceForward = NULL;
            return LFX_SUCCESS;
        } else {
            return LFX_FAILURE;
        }
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Reset() {
        return Forward_LFX_Reset();
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Update() {
        return Forward_LFX_Update();
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_UpdateDefault() {
        return Forward_LFX_UpdateDefault();
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumDevices(unsigned int* const numDevices) {
        return Forward_LFX_GetNumDevices(numDevices);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetDeviceDescription(const unsigned int devIndex, char* const devDesc, const unsigned int devDescSize, unsigned char* const devType) {
        return Forward_LFX_GetDeviceDescription(devIndex, devDesc, devDescSize, devType);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) {
        return Forward_LFX_GetNumLights(devIndex, numLights);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, char* const lightDesc, const unsigned int lightDescSize) {
        return Forward_LFX_GetLightDescription(devIndex, lightIndex, lightDesc, lightDescSize);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) {
        return Forward_LFX_GetLightLocation(devIndex, lightIndex, lightLoc);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) {
        return Forward_LFX_GetLightColor(devIndex, lightIndex, lightCol);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const PLFX_COLOR lightCol) {
        return Forward_LFX_SetLightColor(devIndex, lightIndex, lightCol);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_Light(const unsigned int locationMask, const unsigned int lightCol) {
        return Forward_LFX_Light(locationMask, lightCol);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol) {
        return Forward_LFX_SetLightActionColor(devIndex, lightIndex, actionType, primaryCol);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const PLFX_COLOR primaryCol, const PLFX_COLOR secondaryCol) {
        return Forward_LFX_SetLightActionColorEx(devIndex, lightIndex, actionType, primaryCol, secondaryCol);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColor(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol) {
        return Forward_LFX_ActionColor(locationMask, actionType, primaryCol);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_ActionColorEx(const unsigned int locationMask, const unsigned int actionType, const unsigned int primaryCol, const unsigned int secondaryCol) {
        return Forward_LFX_ActionColorEx(locationMask, actionType, primaryCol, secondaryCol);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_SetTiming(const int newTiming) {
        return Forward_LFX_SetTiming(newTiming);
    }

    FN_DECLSPEC LFX_RESULT STDCALL LFX_GetVersion(char* const version, const unsigned int versionSize) {
        return Forward_LFX_GetVersion(version, versionSize);
    }

    // Custom export for LFXE
    LFXE_API bool LFXE_GetVersion(char* const version, const unsigned int versionSize) {
        return Forward_LFXE_GetVersion(version, versionSize);
    }

#ifdef __cplusplus
}
#endif

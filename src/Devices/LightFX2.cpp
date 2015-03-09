#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightFX2.h"

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>

// Project includes
#include "../Utils/FileIO.h"
#include "../Utils/Windows.h"


using namespace std;
using namespace lightfx::utils;

LFX2INITIALIZE LightFX_Initialize;
LFX2RELEASE LightFX_Release;
LFX2RESET LightFX_Reset;
LFX2UPDATE LightFX_Update;
LFX2UPDATEDEFAULT LightFX_UpdateDefault;
LFX2GETNUMDEVICES LightFX_GetNumDevices;
LFX2GETDEVDESC LightFX_GetDeviceDescription;
LFX2GETNUMLIGHTS LightFX_GetNumLights;
LFX2GETLIGHTDESC LightFX_GetLightDescription;
LFX2GETLIGHTLOC LightFX_GetLightLocation;
LFX2GETLIGHTCOL LightFX_GetLightColor;
LFX2SETLIGHTCOL LightFX_SetLightColor;
LFX2LIGHT LightFX_Light;
LFX2SETLIGHTACTIONCOLOR LightFX_SetLightActionColor;
LFX2SETLIGHTACTIONCOLOREX LightFX_SetLightActionColorEx;
LFX2ACTIONCOLOR LightFX_ActionColor;
LFX2ACTIONCOLOREX LightFX_ActionColorEx;
LFX2SETTIMING LightFX_SetTiming;
LFX2GETVERSION LightFX_GetVersion;

HINSTANCE hInstanceLightFX = NULL;

LFXE_API bool InitializeLightFX() {
    if (hInstanceLightFX) {
        return true;
    }

    // Check if there's a local LightFX_.dll
    if (FileExists(L"LightFX_.dll")) {
        hInstanceLightFX = LoadLibraryW(L"LightFX_.dll");

        // Check if we are accidentally loading ourselves
        if (GetProcAddress(hInstanceLightFX, "LFXE_GetVersion")) {
            FreeLibrary(hInstanceLightFX);
            hInstanceLightFX = NULL;
        }
    }
    
    // Try to load the global one
    if (!hInstanceLightFX) {
        hInstanceLightFX = LoadLibraryW((GetSystemFolder() + L"/LightFX.dll").c_str());

        // Check if we are accidentally loading ourselves
        if (GetProcAddress(hInstanceLightFX, "LFXE_GetVersion")) {
            FreeLibrary(hInstanceLightFX);
            hInstanceLightFX = NULL;
        }
    }

    if (!hInstanceLightFX) {
        return false;
    }

    // Proceed loading addresses of the original LightFX.dll
    LightFX_Initialize = (LFX2INITIALIZE)GetProcAddress(hInstanceLightFX, LFX_DLL_INITIALIZE);
    LightFX_Release = (LFX2RELEASE)GetProcAddress(hInstanceLightFX, LFX_DLL_RELEASE);
    LightFX_Reset = (LFX2RESET)GetProcAddress(hInstanceLightFX, LFX_DLL_RESET);
    LightFX_Update = (LFX2UPDATE)GetProcAddress(hInstanceLightFX, LFX_DLL_UPDATE);
    LightFX_UpdateDefault = (LFX2UPDATEDEFAULT)GetProcAddress(hInstanceLightFX, LFX_DLL_UPDATEDEFAULT);
    LightFX_GetNumDevices = (LFX2GETNUMDEVICES)GetProcAddress(hInstanceLightFX, LFX_DLL_GETNUMDEVICES);
    LightFX_GetDeviceDescription = (LFX2GETDEVDESC)GetProcAddress(hInstanceLightFX, LFX_DLL_GETDEVDESC);
    LightFX_GetNumLights = (LFX2GETNUMLIGHTS)GetProcAddress(hInstanceLightFX, LFX_DLL_GETNUMLIGHTS);
    LightFX_GetLightDescription = (LFX2GETLIGHTDESC)GetProcAddress(hInstanceLightFX, LFX_DLL_GETLIGHTDESC);
    LightFX_GetLightLocation = (LFX2GETLIGHTLOC)GetProcAddress(hInstanceLightFX, LFX_DLL_GETLIGHTLOC);
    LightFX_GetLightColor = (LFX2GETLIGHTCOL)GetProcAddress(hInstanceLightFX, LFX_DLL_GETLIGHTCOL);
    LightFX_SetLightColor = (LFX2SETLIGHTCOL)GetProcAddress(hInstanceLightFX, LFX_DLL_SETLIGHTCOL);
    LightFX_Light = (LFX2LIGHT)GetProcAddress(hInstanceLightFX, LFX_DLL_LIGHT);
    LightFX_SetLightActionColor = (LFX2SETLIGHTACTIONCOLOR)GetProcAddress(hInstanceLightFX, LFX_DLL_SETLIGHTACTIONCOLOR);
    LightFX_SetLightActionColorEx = (LFX2SETLIGHTACTIONCOLOREX)GetProcAddress(hInstanceLightFX, LFX_DLL_SETLIGHTACTIONCOLOREX);
    LightFX_ActionColor = (LFX2ACTIONCOLOR)GetProcAddress(hInstanceLightFX, LFX_DLL_ACTIONCOLOR);
    LightFX_ActionColorEx = (LFX2ACTIONCOLOREX)GetProcAddress(hInstanceLightFX, LFX_DLL_ACTIONCOLOREX);
    LightFX_SetTiming = (LFX2SETTIMING)GetProcAddress(hInstanceLightFX, LFX_DLL_SETTIMING);
    LightFX_GetVersion = (LFX2GETVERSION)GetProcAddress(hInstanceLightFX, LFX_DLL_GETVERSION);
    return true;
}

LFXE_API bool IsLightFXInitialized() {
    return hInstanceLightFX != NULL;
}

LFXE_API bool ReleaseLightFX() {
    if (!hInstanceLightFX) {
        bool result = FreeLibrary(hInstanceLightFX) == TRUE;
        hInstanceLightFX = NULL;
        return result;
    }
    return true;
}

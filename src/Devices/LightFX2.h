#pragma once

// Standard includes
#include <string>

// 3rd party includes
#include "LFX2.h"

// API exports
#include "../Common/ApiExports.h"


extern LFX2INITIALIZE LightFX_Initialize;
extern LFX2RELEASE LightFX_Release;
extern LFX2RESET LightFX_Reset;
extern LFX2UPDATE LightFX_Update;
extern LFX2UPDATEDEFAULT LightFX_UpdateDefault;
extern LFX2GETNUMDEVICES LightFX_GetNumDevices;
extern LFX2GETDEVDESC LightFX_GetDeviceDescription;
extern LFX2GETNUMLIGHTS LightFX_GetNumLights;
extern LFX2GETLIGHTDESC LightFX_GetLightDescription;
extern LFX2GETLIGHTLOC LightFX_GetLightLocation;
extern LFX2GETLIGHTCOL LightFX_GetLightColor;
extern LFX2SETLIGHTCOL LightFX_SetLightColor;
extern LFX2LIGHT LightFX_Light;
extern LFX2SETLIGHTACTIONCOLOR LightFX_SetLightActionColor;
extern LFX2SETLIGHTACTIONCOLOREX LightFX_SetLightActionColorEx;
extern LFX2ACTIONCOLOR LightFX_ActionColor;
extern LFX2ACTIONCOLOREX LightFX_ActionColorEx;
extern LFX2SETTIMING LightFX_SetTiming;
extern LFX2GETVERSION LightFX_GetVersion;

#pragma warning(push)
#pragma warning(disable : 4251)

LFXE_API bool InitializeLightFX(std::wstring name, std::wstring backupName);
LFXE_API bool IsLightFXInitialized();
LFXE_API bool ReleaseLightFX();

LFXE_API std::wstring GetFriendlyLfxResult(LFX_RESULT result);

#pragma warning(pop)

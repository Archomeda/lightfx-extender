#pragma once

// Standard includes
#include <string>

// Windows includes
#include "../../Common/Windows.h"
#include <Windows.h>

// 3rd party includes
#include "LFX2.h"

// Project includes
#include "DeviceLibraryProxy.h"

// API exports
#include "../../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace devices {
        namespace proxies {

            class LFXE_API LightFX2Proxy : DeviceLibraryProxy {

            public:
                LightFX2Proxy(const std::wstring& filename, const std::wstring& backupFilename)
                    : filename(filename), backupFilename(backupFilename) {}

                virtual bool Load() override;
                virtual bool Unload() override;

                using DeviceLibraryProxy::IsLoaded;

                std::wstring LfxResultToString(const LFX_RESULT result);

                // LightFX function declarations
                LFX2INITIALIZE LFX_Initialize;
                LFX2RELEASE LFX_Release;
                LFX2RESET LFX_Reset;
                LFX2UPDATE LFX_Update;
                LFX2UPDATEDEFAULT LFX_UpdateDefault;
                LFX2GETNUMDEVICES LFX_GetNumDevices;
                LFX2GETDEVDESC LFX_GetDeviceDescription;
                LFX2GETNUMLIGHTS LFX_GetNumLights;
                LFX2GETLIGHTDESC LFX_GetLightDescription;
                LFX2GETLIGHTLOC LFX_GetLightLocation;
                LFX2GETLIGHTCOL LFX_GetLightColor;
                LFX2SETLIGHTCOL LFX_SetLightColor;
                LFX2LIGHT LFX_Light;
                LFX2SETLIGHTACTIONCOLOR LFX_SetLightActionColor;
                LFX2SETLIGHTACTIONCOLOREX LFX_SetLightActionColorEx;
                LFX2ACTIONCOLOR LFX_ActionColor;
                LFX2ACTIONCOLOREX LFX_ActionColorEx;
                LFX2SETTIMING LFX_SetTiming;
                LFX2GETVERSION LFX_GetVersion;

            protected:
                bool IsOfficialLightFX(const HINSTANCE hInstance);
                bool ReleaseLibrary();

            private:
                HINSTANCE hInstance = NULL;

                std::wstring filename;
                std::wstring backupFilename;

            };

        }
    }
}

#pragma warning(pop)

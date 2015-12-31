#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightFX2Proxy.h"

// Project includes
#include "../../Utils/FileIO.h"


using namespace std;
using namespace lightfx::utils;


namespace lightfx {
    namespace devices {
        namespace proxies {

            LFXE_API bool LightFX2Proxy::Load() {
                if (this->IsLoaded()) {
                    return true;
                }

                // Check if there's a local DLL
                if (FileExists(this->backupFilename)) {
                    this->hInstance = LoadLibraryW(this->backupFilename.c_str());

                    // Don't load ourselves on accident
                    if (!this->IsOfficialLightFX(this->hInstance)) {
                        this->ReleaseLibrary();
                    }
                }

                // Try to load the global one that's installed in the System32 folder
                if (!this->hInstance) {
                    this->hInstance = LoadLibraryW((GetSystemFolder() + L"\\" + this->filename).c_str());

                    // Don't load ourselves on accident
                    if (!this->IsOfficialLightFX(this->hInstance)) {
                        this->ReleaseLibrary();
                    }
                }

                if (!this->hInstance) {
                    return false;
                }

                // Load the addresses of the original LightFX.dll
                //! This needs to be updated every time the LightFX DLL has been updated
                this->LFX_Initialize = (LFX2INITIALIZE)GetProcAddress(this->hInstance, LFX_DLL_INITIALIZE);
                this->LFX_Release = (LFX2RELEASE)GetProcAddress(this->hInstance, LFX_DLL_RELEASE);
                this->LFX_Reset = (LFX2RESET)GetProcAddress(this->hInstance, LFX_DLL_RESET);
                this->LFX_Update = (LFX2UPDATE)GetProcAddress(this->hInstance, LFX_DLL_UPDATE);
                this->LFX_UpdateDefault = (LFX2UPDATEDEFAULT)GetProcAddress(this->hInstance, LFX_DLL_UPDATEDEFAULT);
                this->LFX_GetNumDevices = (LFX2GETNUMDEVICES)GetProcAddress(this->hInstance, LFX_DLL_GETNUMDEVICES);
                this->LFX_GetDeviceDescription = (LFX2GETDEVDESC)GetProcAddress(this->hInstance, LFX_DLL_GETDEVDESC);
                this->LFX_GetNumLights = (LFX2GETNUMLIGHTS)GetProcAddress(this->hInstance, LFX_DLL_GETNUMLIGHTS);
                this->LFX_GetLightDescription = (LFX2GETLIGHTDESC)GetProcAddress(this->hInstance, LFX_DLL_GETLIGHTDESC);
                this->LFX_GetLightLocation = (LFX2GETLIGHTLOC)GetProcAddress(this->hInstance, LFX_DLL_GETLIGHTLOC);
                this->LFX_GetLightColor = (LFX2GETLIGHTCOL)GetProcAddress(this->hInstance, LFX_DLL_GETLIGHTCOL);
                this->LFX_SetLightColor = (LFX2SETLIGHTCOL)GetProcAddress(this->hInstance, LFX_DLL_SETLIGHTCOL);
                this->LFX_Light = (LFX2LIGHT)GetProcAddress(this->hInstance, LFX_DLL_LIGHT);
                this->LFX_SetLightActionColor = (LFX2SETLIGHTACTIONCOLOR)GetProcAddress(this->hInstance, LFX_DLL_SETLIGHTACTIONCOLOR);
                this->LFX_SetLightActionColorEx = (LFX2SETLIGHTACTIONCOLOREX)GetProcAddress(this->hInstance, LFX_DLL_SETLIGHTACTIONCOLOREX);
                this->LFX_ActionColor = (LFX2ACTIONCOLOR)GetProcAddress(this->hInstance, LFX_DLL_ACTIONCOLOR);
                this->LFX_ActionColorEx = (LFX2ACTIONCOLOREX)GetProcAddress(this->hInstance, LFX_DLL_ACTIONCOLOREX);
                this->LFX_SetTiming = (LFX2SETTIMING)GetProcAddress(this->hInstance, LFX_DLL_SETTIMING);
                this->LFX_GetVersion = (LFX2GETVERSION)GetProcAddress(this->hInstance, LFX_DLL_GETVERSION);

                this->SetLoaded(true);
                return true;
            }

            LFXE_API bool LightFX2Proxy::Unload() {
                if (this->IsLoaded()) {
                    bool result = this->ReleaseLibrary();
                    this->SetLoaded(!result);
                    return result;
                }
                return true;
            }

            LFXE_API wstring LightFX2Proxy::LfxResultToString(const LFX_RESULT result) {
                switch (result)
                {
                case LFX_SUCCESS:
                    return L"LFX_SUCCESS";
                case LFX_FAILURE:
                    return L"LFX_FAILURE";
                case LFX_ERROR_NOINIT:
                    return L"LFX_ERROR_NOINIT";
                case LFX_ERROR_NODEVS:
                    return L"LFX_ERROR_NODEVS";
                case LFX_ERROR_NOLIGHTS:
                    return L"LFX_ERROR_NOLIGHTS";
                case LFX_ERROR_BUFFSIZE:
                    return L"LFX_ERROR_BUFFSIZE";
                default:
                    return L"Unknown error " + to_wstring(result);
                }
            }

            LFXE_API bool LightFX2Proxy::IsOfficialLightFX(const HINSTANCE hInstance) {
                return GetProcAddress(hInstance, "LFXE_GetVersion") == NULL;
            }

            LFXE_API bool LightFX2Proxy::ReleaseLibrary() {
                if (this->hInstance) {
                    bool result = FreeLibrary(this->hInstance) == TRUE;
                    this->hInstance = NULL;
                    return result;
                }
                return true;
            }
        }
    }
}

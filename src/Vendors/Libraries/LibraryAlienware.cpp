#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LibraryAlienware.h"

// Project includes
#include "../../Utils/FileIO.h"
#include "../../Utils/String.h"
#include "../../Utils/Log.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {
    namespace vendors {
        namespace libraries {

            LFXE_API LibraryAlienware::~LibraryAlienware() {
                this->ReleaseLibrary();
            }

            LFXE_API bool LibraryAlienware::IsLibraryAvailable() const {
                // We have to load the library before we can query the SDK version
                // So we initialize if we aren't initialized yet
                // If we are already initialized, well... then the library is available isn't it?
                if (!this->isInitialized) {
                    bool result = this->LoadDll();
                    this->UnloadDll();
                    return result;
                }
                return true;
            }

            LFXE_API bool LibraryAlienware::InitializeLibrary() {
                if (!this->isInitialized) {
                    LOG_INFO(L"Initializing Alienware library...");
                    bool loaded = this->LoadDll() && this->LoadExports();
                    if (loaded) {
                        AlienwareResult initialized = this->Aw_Initialize();
                        if (initialized == AlienwareResult::AlienwareSuccess) {
                            this->isInitialized = true;
                            LOG_INFO(L"Alienware library initialized");
                        } else {
                            LOG_WARNING(L"Could not initialize Alienware library: " + this->AlienwareResultToString(initialized));
                        }
                    } else {
                        LOG_WARNING(L"Could not load Alienware library");
                    }
                }
                return this->isInitialized;
            }

            LFXE_API bool LibraryAlienware::ReleaseLibrary() {
                if (this->isInitialized) {
                    LOG_INFO(L"Releasing Alienware library...");
                    this->Aw_Release();
                    this->UnloadExports();
                    this->UnloadDll();
                    this->isInitialized = false;
                    LOG_INFO(L"Alienware library released");
                }
                return !this->isInitialized;
            }

            LFXE_API bool LibraryAlienware::LoadDll() const {
                if (this->hInstance) {
                    return true;
                }

                // Try to load the file with our backup filename first
                this->hInstance = LoadLibraryW(this->backupFilename.c_str());
                if (this->hInstance && !GetProcAddress(this->hInstance, "LFXE_GetVersion")) {
                    return true;
                }

                // Try to load the default LightFX.dll from the system folder
                this->hInstance = LoadLibraryW((GetSystemFolder() + L"LightFX.dll").c_str());
                if (this->hInstance && !GetProcAddress(this->hInstance, "LFXE_GetVersion")) {
                    return true;
                }

                return false;
            }

            LFXE_API bool LibraryAlienware::LoadExports() {
                if (this->hInstance) {
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
                    return true;
                }
                return false;
            }

            LFXE_API bool LibraryAlienware::UnloadDll() const {
                if (!this->hInstance) {
                    FreeLibrary(this->hInstance);
                    this->hInstance = NULL;
                }
                return true;
            }

            LFXE_API bool LibraryAlienware::UnloadExports() {
                this->LFX_Initialize = NULL;
                this->LFX_Release = NULL;
                this->LFX_Reset = NULL;
                this->LFX_Update = NULL;
                this->LFX_UpdateDefault = NULL;
                this->LFX_GetNumDevices = NULL;
                this->LFX_GetDeviceDescription = NULL;
                this->LFX_GetNumLights = NULL;
                this->LFX_GetLightDescription = NULL;
                this->LFX_GetLightLocation = NULL;
                this->LFX_GetLightColor = NULL;
                this->LFX_SetLightColor = NULL;
                this->LFX_Light = NULL;
                this->LFX_SetLightActionColor = NULL;
                this->LFX_SetLightActionColorEx = NULL;
                this->LFX_ActionColor = NULL;
                this->LFX_ActionColorEx = NULL;
                this->LFX_SetTiming = NULL;
                this->LFX_GetVersion = NULL;
                return true;
            }

            LFXE_API Version LibraryAlienware::GetLibraryVersion() const {
                // We have to load the library before we can query the SDK version
                // So if we aren't loaded, let's do that first and shut down afterwards
                Version version;
                if (!this->isInitialized) {
                    bool result = this->LoadDll();
                    if (result) {
                        LFX2GETVERSION getVersion = (LFX2GETVERSION)GetProcAddress(this->hInstance, LFX_DLL_GETVERSION);
                        if (getVersion) {
                            char buff[LFX_MAX_STRING_SIZE];
                            AlienwareResult result = static_cast<AlienwareResult>(getVersion(buff, sizeof(buff)));
                            if (result == AlienwareResult::AlienwareSuccess) {
                                version = Version::FromString(buff);
                            }
                        }
                        this->UnloadDll();
                    } else {
                        this->UnloadDll();
                        return{};
                    }
                } else {
                    wstring buff;
                    if (this->Aw_GetVersion(&buff) == AlienwareResult::AlienwareSuccess) {
                        version = Version::FromString(buff);
                    }
                }
                return version;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_Initialize() const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_Initialize());
                LOG_DEBUG(L"LFX_Initialize(): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_Release() const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_Release());
                LOG_DEBUG(L"LFX_Release(): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_Reset() const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_Reset());
                LOG_DEBUG(L"LFX_Reset(): " + this->AlienwareResultToString(result));
                return result;
            }
            
            LFXE_API AlienwareResult LibraryAlienware::Aw_Update() const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_Update());
                LOG_DEBUG(L"LFX_Update(): " + this->AlienwareResultToString(result));
                return result;
            }
            
            LFXE_API AlienwareResult LibraryAlienware::Aw_UpdateDefault() const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_UpdateDefault());
                LOG_DEBUG(L"LFX_UpdateDefault(): " + this->AlienwareResultToString(result));
                return result;
            }
            
            LFXE_API AlienwareResult LibraryAlienware::Aw_GetNumDevices(unsigned int* const numDevices) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_GetNumDevices(numDevices));
                LOG_DEBUG(L"LFX_GetNumDevices(" + to_wstring(*numDevices) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_GetDeviceDescription(const unsigned int devIndex, wstring* const devDesc, AlienwareDeviceType* const devType) const {
                char devDescChar[LFX_MAX_STRING_SIZE];
                unsigned char devTypeChar;
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_GetDeviceDescription(devIndex, devDescChar, sizeof(devDescChar), &devTypeChar));
                *devDesc = string_to_wstring(devDescChar);
                *devType = static_cast<AlienwareDeviceType>(devTypeChar);
                LOG_DEBUG(L"LFX_GetDeviceDescription(" + to_wstring(devIndex) + L", \"" + *devDesc + L"\", " + to_wstring(sizeof(devDescChar)) + L", " + to_wstring(*devType) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_GetNumLights(devIndex, numLights));
                LOG_DEBUG(L"LFX_GetNumLights(" + to_wstring(devIndex) + L", " + to_wstring(*numLights) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, wstring* const lightDesc) const {
                char lightDescChar[LFX_MAX_STRING_SIZE];
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_GetLightDescription(devIndex, lightIndex, lightDescChar, sizeof(lightDescChar)));
                *lightDesc = string_to_wstring(lightDescChar);
                LOG_DEBUG(L"LFX_GetLightDescription(" + to_wstring(devIndex) + L", " + to_wstring(lightIndex) + L", \"" + *lightDesc + L"\", " + to_wstring(sizeof(lightDescChar)) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_GetLightLocation(devIndex, lightIndex, lightLoc));
                LOG_DEBUG(L"LFX_GetLightLocation(" + to_wstring(devIndex) + L", " + to_wstring(lightIndex) + L", ...): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_GetLightColor(devIndex, lightIndex, lightCol));
                LOG_DEBUG(L"LFX_GetLightColor(" + to_wstring(devIndex) + L", " + to_wstring(lightIndex) + L", ...): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const LFX_COLOR& lightCol) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_SetLightColor(devIndex, lightIndex, (PLFX_COLOR)&lightCol));
                LOG_DEBUG(L"LFX_SetLightColor(" + to_wstring(devIndex) + L", " + to_wstring(lightIndex) + L", ...): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_Light(const unsigned int locationMask, const LFX_COLOR& lightCol) const {
                unsigned int lightColUInt = this->LfxColorToUInt(lightCol);
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_Light(locationMask, lightColUInt));
                LOG_DEBUG(L"LFX_SetLightColor(" + to_wstring(locationMask) + L", " + to_wstring(lightColUInt) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const AlienwareActionType actionType, const LFX_COLOR& primaryCol) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_SetLightActionColor(devIndex, lightIndex, actionType, (PLFX_COLOR)&primaryCol));
                LOG_DEBUG(L"LFX_SetLightColor(" + to_wstring(devIndex) + L", " + to_wstring(lightIndex) + L", " + to_wstring(actionType) + L"...): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const AlienwareActionType actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_SetLightActionColorEx(devIndex, lightIndex, actionType, (PLFX_COLOR)&primaryCol, (PLFX_COLOR)&secondaryCol));
                LOG_DEBUG(L"LFX_SetLightActionColorEx(" + to_wstring(devIndex) + L", " + to_wstring(lightIndex) + L", " + to_wstring(actionType) + L"..., ...): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_ActionColor(const unsigned int locationMask, const AlienwareActionType actionType, const LFX_COLOR& primaryCol) const {
                unsigned int primaryColUInt = this->LfxColorToUInt(primaryCol);
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_ActionColor(locationMask, actionType, primaryColUInt));
                LOG_DEBUG(L"LFX_ActionColor(" + to_wstring(locationMask) + L", " + to_wstring(actionType) + L", " + to_wstring(primaryColUInt) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_ActionColorEx(const unsigned int locationMask, const AlienwareActionType actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) const {
                unsigned int primaryColUInt = this->LfxColorToUInt(primaryCol);
                unsigned int secondaryColUInt = this->LfxColorToUInt(secondaryCol);
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_ActionColorEx(locationMask, actionType, primaryColUInt, secondaryColUInt));
                LOG_DEBUG(L"LFX_ActionColorEx(" + to_wstring(locationMask) + L", " + to_wstring(actionType) + L", " + to_wstring(primaryColUInt) + L", " + to_wstring(secondaryColUInt) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_SetTiming(const int newTiming) const {
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_SetTiming(newTiming));
                LOG_DEBUG(L"LFX_SetTiming(" + to_wstring(newTiming) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API AlienwareResult LibraryAlienware::Aw_GetVersion(wstring* const version) const {
                char versionChar[LFX_MAX_STRING_SIZE];
                AlienwareResult result = static_cast<AlienwareResult>(this->LFX_GetVersion(versionChar, sizeof(versionChar)));
                *version = string_to_wstring(versionChar);
                LOG_DEBUG(L"LFX_GetVersion(\"" + *version + L"\", " + to_wstring(sizeof(versionChar)) + L"): " + this->AlienwareResultToString(result));
                return result;
            }

            LFXE_API LFX_COLOR LibraryAlienware::UIntToLfxColor(const unsigned int color) const {
                LFX_COLOR c;
                c.brightness = (color >> 24) & 0xFF;
                c.red = (color >> 16) & 0xFF;
                c.green = (color >> 8) & 0xFF;
                c.blue = color & 0xFF;
                return c;
            }

            LFXE_API unsigned int LibraryAlienware::LfxColorToUInt(const LFX_COLOR color) const {
                return (color.blue & 0xFF) | ((color.green & 0xFF) << 8) | ((color.red & 0xFF) << 16) | ((color.brightness & 0xFF) << 24);
            }

            LFXE_API wstring LibraryAlienware::AlienwareResultToString(const AlienwareResult result) const {
                switch (result) {
                case AlienwareSuccess:
                    return L"AlienwareSuccess";
                case AlienwareFailure:
                    return L"AlienwareFailure";
                case AlienwareErrorNoInit:
                    return L"AlienwareErrorNoInit";
                case AlienwareErrorNoDevices:
                    return L"AlienwareErrorNoDevices";
                case AlienwareErrorNoLights:
                    return L"AlienwareErrorNoLights";
                case AlienwareErrorBufferSizeTooSmall:
                    return L"AlienwareErrorBufferSizeTooSmall";
                default:
                    return L"Unknown error " + to_wstring(result);
                }
            }

        }
    }
}

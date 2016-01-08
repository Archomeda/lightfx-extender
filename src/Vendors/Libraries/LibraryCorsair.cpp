#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LibraryCorsair.h"

// Project includes
#include "../../Utils/FileIO.h"
#include "../../Utils/String.h"
#include "../../Utils/Log.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {
    namespace vendors {
        namespace libraries {

            LFXE_API LibraryCorsair::~LibraryCorsair() {
                this->ReleaseLibrary();
            }

            LFXE_API bool LibraryCorsair::IsLibraryAvailable() const {
                // We have to load the library and check if we can connect to it properly
                // So we initialize if we aren't initialized yet
                // If we are already initialized, well... then the library is available isn't it?
                if (!this->isInitialized) {
                    bool result = this->LoadDll();
                    if (result) {
                        CORSAIRPERFORMPROTOCOLHANDSHAKE handshake = (CORSAIRPERFORMPROTOCOLHANDSHAKE)GetProcAddress(this->hInstance, CORSAIR_DLL_PERFORMPROTOCOLHANDSHAKE);
                        CORSAIRGETLASTERROR lastError = (CORSAIRGETLASTERROR)GetProcAddress(this->hInstance, CORSAIR_DLL_GETLASTERROR);
                        if (handshake && lastError) {
                            handshake();
                            result = lastError() == CorsairError::CE_Success;
                        }
                    }
                    this->UnloadDll();
                    return result;
                }
                return true;
            }

            LFXE_API bool LibraryCorsair::InitializeLibrary() {
                if (!this->isInitialized) {
                    LOG_INFO(L"Initializing Corsair library...");
                    bool loaded = this->LoadDll() && this->LoadExports();
                    if (loaded) {
                        CorsairProtocolDetails protocol = this->Cs_PerformProtocolHandshake();
                        if (protocol.breakingChanges) {
                            LOG_WARNING(L"Breaking changes detected in Corsair SDK, please submit an issue on GitHub to update the SDK");
                        }
                        CorsairError error = this->Cs_GetLastError();
                        if (error == CorsairError::CE_Success) {
                            this->isInitialized = true;
                            LOG_INFO(L"Corsair library initialized");
                        } else {
                            LOG_WARNING(L"Could not initialize Corsair library: " + this->CorsairErrorToString(error));
                        }
                    } else {
                        LOG_WARNING(L"Could not load Corsair library");
                    }
                }
                return this->isInitialized;
            }

            LFXE_API bool LibraryCorsair::ReleaseLibrary() {
                if (this->isInitialized) {
                    LOG_INFO(L"Releasing Corsair library...");
                    this->UnloadExports();
                    this->UnloadDll();
                    this->isInitialized = false;
                    LOG_INFO(L"Corsair library released");
                }
                return !this->isInitialized;
            }

            LFXE_API bool LibraryCorsair::LoadDll() const {
                if (this->hInstance) {
                    return true;
                }
                this->hInstance = LoadLibraryW(CORSAIR_DLL_NAME);
                return this->hInstance != NULL;
            }

            LFXE_API bool LibraryCorsair::LoadExports() {
                if (this->hInstance) {
                    this->CorsairSetLedsColors = (CORSAIRSETLEDSCOLORS)GetProcAddress(this->hInstance, CORSAIR_DLL_SETLEDSCOLORS);
                    this->CorsairSetLedsColorsAsync = (CORSAIRSETLEDSCOLORSASYNC)GetProcAddress(this->hInstance, CORSAIR_DLL_SETLEDSCOLORSASYNC);
                    this->CorsairGetDeviceCount = (CORSAIRGETDEVICECOUNT)GetProcAddress(this->hInstance, CORSAIR_DLL_GETDEVICECOUNT);
                    this->CorsairGetDeviceInfo = (CORSAIRGETDEVICEINFO)GetProcAddress(this->hInstance, CORSAIR_DLL_GETDEVICEINFO);
                    this->CorsairGetLedPositions = (CORSAIRGETLEDPOSITIONS)GetProcAddress(this->hInstance, CORSAIR_DLL_GETLEDPOSITIONS);
                    this->CorsairGetLedIdForKeyName = (CORSAIRGETLEDIDFORKEYNAME)GetProcAddress(this->hInstance, CORSAIR_DLL_GETLEDIDFORKEYNAME);
                    this->CorsairRequestControl = (CORSAIRREQUESTCONTROL)GetProcAddress(this->hInstance, CORSAIR_DLL_REQUESTCONTROL);
                    this->CorsairPerformProtocolHandshake = (CORSAIRPERFORMPROTOCOLHANDSHAKE)GetProcAddress(this->hInstance, CORSAIR_DLL_PERFORMPROTOCOLHANDSHAKE);
                    this->CorsairGetLastError = (CORSAIRGETLASTERROR)GetProcAddress(this->hInstance, CORSAIR_DLL_GETLASTERROR);
                    return true;
                }
                return false;
            }

            LFXE_API bool LibraryCorsair::UnloadDll() const {
                if (!this->hInstance) {
                    FreeLibrary(this->hInstance);
                    this->hInstance = NULL;
                }
                return true;
            }

            LFXE_API bool LibraryCorsair::UnloadExports() {
                this->CorsairSetLedsColors = NULL;
                this->CorsairSetLedsColorsAsync = NULL;
                this->CorsairGetDeviceCount = NULL;
                this->CorsairGetDeviceInfo = NULL;
                this->CorsairGetLedPositions = NULL;
                this->CorsairGetLedIdForKeyName = NULL;
                this->CorsairRequestControl = NULL;
                this->CorsairPerformProtocolHandshake = NULL;
                this->CorsairGetLastError = NULL;
                return true;
            }

            LFXE_API Version LibraryCorsair::GetLibraryVersion() const {
                // We have to perform a handshake with the library before we can get the SDK version
                // So if we aren't loaded, let's do that first and shut down afterwards
                Version version;
                if (!this->isInitialized) {
                    bool result = this->LoadDll();
                    if (result) {
                        CORSAIRPERFORMPROTOCOLHANDSHAKE handshake = (CORSAIRPERFORMPROTOCOLHANDSHAKE)GetProcAddress(this->hInstance, CORSAIR_DLL_PERFORMPROTOCOLHANDSHAKE);
                        if (handshake) {
                            CorsairProtocolDetails protocol = handshake();
                            version = Version::FromString(protocol.sdkVersion);
                        }
                        this->UnloadDll();
                    } else {
                        this->UnloadDll();
                        return{};
                    }
                } else {
                    CorsairProtocolDetails protocol = this->Cs_PerformProtocolHandshake();
                    version = Version::FromString(protocol.sdkVersion);
                }
                return version;
            }

            LFXE_API bool LibraryCorsair::Cs_SetLedsColors(int size, CorsairLedColor* ledsColors) const {
                bool result = this->CorsairSetLedsColors(size, ledsColors);
                LOG_DEBUG(L"CorsairSetLedsColors(" + to_wstring(size) + L", ...): " + to_wstring(result));
                return result;
            }

            LFXE_API bool LibraryCorsair::Cs_SetLedsColorsAsync(int size, CorsairLedColor* ledsColors, void(*CallbackType)(void*, bool, CorsairError), void* context) const {
                bool result = this->CorsairSetLedsColorsAsync(size, ledsColors, CallbackType, context);
                LOG_DEBUG(L"CorsairSetLedsColorsAsync(" + to_wstring(size) + L", ..., ..., ...): " + to_wstring(result));
                return result;
            }

            LFXE_API int LibraryCorsair::Cs_GetDeviceCount() const {
                int result = this->CorsairGetDeviceCount();
                LOG_DEBUG(L"CorsairGetDeviceCount(): " + to_wstring(result));
                return result;
            }

            LFXE_API CorsairDeviceInfo* LibraryCorsair::Cs_GetDeviceInfo(int deviceIndex) const {
                CorsairDeviceInfo* result = this->CorsairGetDeviceInfo(deviceIndex);
                LOG_DEBUG(L"CorsairGetDeviceInfo(" + to_wstring(deviceIndex) + L"): \"" + string_to_wstring(result->model) + L"\"");
                return result;
            }

            LFXE_API CorsairLedPositions* LibraryCorsair::Cs_GetLedPositions() const {
                CorsairLedPositions* result = this->CorsairGetLedPositions();
                LOG_DEBUG(L"CorsairGetLedPositions(): " + to_wstring(result->numberOfLed) + L" leds");
                return result;
            }

            LFXE_API CorsairLedId LibraryCorsair::Cs_GetLedIdForKeyName(char keyName) const {
                CorsairLedId result = this->CorsairGetLedIdForKeyName(keyName);
                LOG_DEBUG(L"CorsairGetLedIdForKeyName(\"" + to_wstring(keyName) + L"\"): " + to_wstring(result));
                return result;
            }

            LFXE_API bool LibraryCorsair::Cs_RequestControl(CorsairAccessMode accessMode) const {
                bool result = this->CorsairRequestControl(accessMode);
                LOG_DEBUG(L"CorsairRequestControl(" + to_wstring(accessMode) + L"): " + to_wstring(result));
                return result;
            }

            LFXE_API CorsairProtocolDetails LibraryCorsair::Cs_PerformProtocolHandshake() const {
                CorsairProtocolDetails result = this->CorsairPerformProtocolHandshake();
                LOG_DEBUG(L"CorsairPerformProtocolHandshake(): SDK " + string_to_wstring(result.sdkVersion) + L" - Server " + string_to_wstring(result.serverVersion));
                return result;
            }

            LFXE_API CorsairError LibraryCorsair::Cs_GetLastError() const {
                CorsairError result = this->CorsairGetLastError();
                LOG_DEBUG(L"CorsairGetLastError(): " + this->CorsairErrorToString(result));
                return result;
            }

            LFXE_API wstring LibraryCorsair::CorsairErrorToString(const CorsairError result) const {
                switch (result) {
                case CE_Success:
                    return L"CE_Success";
                case CE_ServerNotFound:
                    return L"CE_ServerNotFound";
                case CE_NoControl:
                    return L"CE_NoControl";
                case CE_ProtocolHandshakeMissing:
                    return L"CE_ProtocolHandshakeMissing";
                case CE_IncompatibleProtocol:
                    return L"CE_IncompatibleProtocol";
                case CE_InvalidArguments:
                    return L"CE_InvalidArguments";
                default:
                    return L"Unknown error " + to_wstring(result);
                }
            }

        }
    }
}

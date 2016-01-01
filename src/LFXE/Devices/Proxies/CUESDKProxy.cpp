#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "CUESDKProxy.h"


using namespace std;


namespace lightfx {
    namespace devices {
        namespace proxies {

            LFXE_API bool CUESDKProxy::Load() {
                if (this->IsLoaded()) {
                    return true;
                }

                // Try to load the library if there's a local DLL
                this->hInstance = LoadLibraryA(CORSAIR_DLL_NAME);

                if (!this->hInstance) {
                    return false;
                }

                // Load the function addresses
                //! This needs to be updated every time the CUESDK DLL has been updated
                this->CorsairSetLedsColors = (CORSAIRSETLEDSCOLORS)GetProcAddress(this->hInstance, CORSAIR_DLL_SETLEDSCOLORS);
                this->CorsairSetLedsColorsAsync = (CORSAIRSETLEDSCOLORSASYNC)GetProcAddress(this->hInstance, CORSAIR_DLL_SETLEDSCOLORSASYNC);
                this->CorsairGetDeviceCount = (CORSAIRGETDEVICECOUNT)GetProcAddress(this->hInstance, CORSAIR_DLL_GETDEVICECOUNT);
                this->CorsairGetDeviceInfo = (CORSAIRGETDEVICEINFO)GetProcAddress(this->hInstance, CORSAIR_DLL_GETDEVICEINFO);
                this->CorsairGetLedPositions = (CORSAIRGETLEDPOSITIONS)GetProcAddress(this->hInstance, CORSAIR_DLL_GETLEDPOSITIONS);
                this->CorsairGetLedIdForKeyName = (CORSAIRGETLEDIDFORKEYNAME)GetProcAddress(this->hInstance, CORSAIR_DLL_GETLEDIDFORKEYNAME);
                this->CorsairRequestControl = (CORSAIRREQUESTCONTROL)GetProcAddress(this->hInstance, CORSAIR_DLL_REQUESTCONTROL);
                this->CorsairPerformProtocolHandshake = (CORSAIRPERFORMPROTOCOLHANDSHAKE)GetProcAddress(this->hInstance, CORSAIR_DLL_PERFORMPROTOCOLHANDSHAKE);
                this->CorsairGetLastError = (CORSAIRGETLASTERROR)GetProcAddress(this->hInstance, CORSAIR_DLL_GETLASTERROR);

                this->SetLoaded(true);
                return true;
            }

            LFXE_API bool CUESDKProxy::Unload() {
                if (this->IsLoaded()) {
                    bool result = this->ReleaseLibrary();
                    this->SetLoaded(!result);
                    return result;
                }
                return true;
            }

            LFXE_API wstring CUESDKProxy::CorsairErrorToString(const CorsairError error) {
                switch (error) {
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
                    return L"Unknown error " + to_wstring(error);
                }
            }

            LFXE_API bool CUESDKProxy::ReleaseLibrary() {
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

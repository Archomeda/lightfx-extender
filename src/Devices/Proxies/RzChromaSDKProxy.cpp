#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "RzChromaSDKProxy.h"


using namespace std;

namespace lightfx {
    namespace devices {
        namespace proxies {

            LFXE_API bool RzChromaSDKProxy::Load() {
                if (this->IsLoaded()) {
                    return true;
                }

                // Try to load the library if there's a local DLL
                this->hInstance = LoadLibraryA(RZ_DLL_NAME);

                if (!this->hInstance) {
                    return false;
                }

                // Load the function addresses
                //! This needs to be updated every time the Razor Chroma DLL has been updated
                this->RzInit = (RZINIT)GetProcAddress(this->hInstance, RZ_DLL_INIT);
                this->RzUnInit = (RZUNINIT)GetProcAddress(this->hInstance, RZ_DLL_UNINIT);
                this->RzCreateEffect = (RZCREATEEFFECT)GetProcAddress(this->hInstance, RZ_DLL_CREATEEFFECT);
                this->RzCreateKeyboardEffect = (RZCREATEKEYBOARDEFFECT)GetProcAddress(this->hInstance, RZ_DLL_CREATEKEYBOARDEFFECT);
                this->RzCreateHeadsetEffect = (RZCREATEHEADSETEFFECT)GetProcAddress(this->hInstance, RZ_DLL_CREATEHEADSETEFFECT);
                this->RzCreateMousepadEffect = (RZCREATEMOUSEPADEFFECT)GetProcAddress(this->hInstance, RZ_DLL_CREATEMOUSEPADEFFECT);
                this->RzCreateMouseEffect = (RZCREATEMOUSEEFFECT)GetProcAddress(this->hInstance, RZ_DLL_CREATEMOUSEEFFECT);
                this->RzCreateKeypadEffect = (RZCREATEKEYPADEFFECT)GetProcAddress(this->hInstance, RZ_DLL_CREATEKEYPADEFFECT);
                this->RzSetEffect = (RZSETEFFECT)GetProcAddress(this->hInstance, RZ_DLL_SETEFFECT);
                this->RzDeleteEffect = (RZDELETEEFFECT)GetProcAddress(this->hInstance, RZ_DLL_DELETEEFFECT);
                this->RzQueryDevice = (RZQUERYDEVICE)GetProcAddress(this->hInstance, RZ_DLL_QUERYDEVICE);

                this->SetLoaded(true);
                return true;
            }

            LFXE_API bool RzChromaSDKProxy::Unload() {
                if (this->IsLoaded()) {
                    bool result = this->ReleaseLibrary();
                    this->SetLoaded(!result);
                    return result;
                }
                return true;
            }

            LFXE_API wstring RzChromaSDKProxy::RzResultToString(const RZRESULT result) {
                switch (result) {
                case RZRESULT_INVALID:
                    return L"RZRESULT_INVALID";
                case RZRESULT_SUCCESS:
                    return L"RZRESULT_SUCCESS";
                case RZRESULT_ACCESS_DENIED:
                    return L"RZRESULT_ACCESS_DENIED";
                case RZRESULT_INVALID_HANDLE:
                    return L"RZRESULT_INVALID_HANDLE";
                case RZRESULT_NOT_SUPPORTED:
                    return L"RZRESULT_NOT_SUPPORTED";
                case RZRESULT_INVALID_PARAMETER:
                    return L"RZRESULT_INVALID_PARAMETER";
                case RZRESULT_SERVICE_NOT_ACTIVE:
                    return L"RZRESULT_SERVICE_NOT_ACTIVE";
                case RZRESULT_SINGLE_INSTANCE_APP:
                    return L"RZRESULT_SINGLE_INSTANCE_APP";
                case RZRESULT_DEVICE_NOT_CONNECTED:
                    return L"RZRESULT_DEVICE_NOT_CONNECTED";
                case RZRESULT_NOT_FOUND:
                    return L"RZRESULT_NOT_FOUND";
                case RZRESULT_REQUEST_ABORTED:
                    return L"RZRESULT_REQUEST_ABORTED";
                case RZRESULT_ALREADY_INITIALIZED:
                    return L"RZRESULT_ALREADY_INITIALIZED";
                case RZRESULT_RESOURCE_DISABLED:
                    return L"RZRESULT_RESOURCE_DISABLED";
                case RZRESULT_DEVICE_NOT_AVAILABLE:
                    return L"RZRESULT_DEVICE_NOT_AVAILABLE";
                case RZRESULT_NOT_VALID_STATE:
                    return L"RZRESULT_NOT_VALID_STATE";
                case RZRESULT_FAILED:
                    return L"RZRESULT_FAILED";
                default:
                    return L"Unknown error " + to_wstring(result);
                }
            }

            LFXE_API bool RzChromaSDKProxy::ReleaseLibrary() {
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

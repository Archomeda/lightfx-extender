#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceLibraryProxy.h"


namespace lightfx {
    namespace devices {
        namespace proxies {

            LFXE_API DeviceLibraryProxy::~DeviceLibraryProxy() {
                this->Unload();
            }

            LFXE_API bool DeviceLibraryProxy::IsLoaded() {
                return this->isLoaded;
            }

            LFXE_API bool DeviceLibraryProxy::Load() {
                return false;
            }

            LFXE_API bool DeviceLibraryProxy::Unload() {
                return false;
            }

            LFXE_API void DeviceLibraryProxy::SetLoaded(const bool loaded) {
                this->isLoaded = loaded;
            }

        }
    }
}

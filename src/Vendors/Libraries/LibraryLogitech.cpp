#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LibraryLogitech.h"

// Project includes
#include "../../Utils/Log.h"


using namespace std;
using namespace lightfx;

namespace lightfx {
    namespace vendors {
        namespace libraries {

            LFXE_API LibraryLogitech::~LibraryLogitech() {
                this->ReleaseLibrary();
            }

            LFXE_API bool LibraryLogitech::IsLibraryAvailable() const {
                // Sadly, we have to initialize before we can query the SDK version
                // So instead, we hack our way around this by just initializing if we aren't initialized yet
                // If we are already initialized, well... then the library is available isn't it?
                if (!this->isInitialized) {
                    bool result = this->Logi_Init();
                    this->Logi_Shutdown();
                    return result;
                }
                return true;
            }

            LFXE_API bool LibraryLogitech::InitializeLibrary() {
                if (!this->isInitialized) {
                    LOG_INFO(L"Initializing Logitech library...");
                    this->isInitialized = this->Logi_Init();
                    if (this->isInitialized) {
                        LOG_INFO(L"Logitech library initialized");
                    } else {
                        LOG_WARNING(L"Could not initialize Logitech library");
                    }
                }
                return this->isInitialized;
            }

            LFXE_API bool LibraryLogitech::ReleaseLibrary() {
                if (this->isInitialized) {
                    LOG_INFO(L"Releasing Logitech library...");
                    this->Logi_SetTargetDevice(LOGI_DEVICETYPE_ALL);
                    this->Logi_RestoreLighting();
                    this->Logi_Shutdown();
                    this->isInitialized = false;
                    LOG_INFO(L"Logitech library released");
                }
                return true;
            }

            LFXE_API Version LibraryLogitech::GetLibraryVerion() const {
                // Sadly, we have to initialize before we can query the SDK version
                // So if we aren't initialized, let's do that first and shut down afterwards
                if (!this->isInitialized) {
                    bool result = this->Logi_Init();
                    if (!result) {
                        return{};
                    }
                }
                int major = 0, minor = 0, build = 0;
                Version version;
                if (this->Logi_GetSdkVersion(&major, &minor, &build)) {
                    version = Version(major, minor, build, 0);
                }
                if (!this->isInitialized) {
                    this->Logi_Shutdown();
                }
                return version;
            }

        }
    }
}

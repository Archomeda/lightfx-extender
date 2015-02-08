#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "UpdateManager.h"


using namespace std;

namespace lightfx {
    namespace managers {

        LFXE_API UpdateManager::UpdateManager(const LightFXExtender* lightFXExtender) {

        }

        LFXE_API UpdateManager::~UpdateManager() {

        }


        LFXE_API Version UpdateManager::GetCurrentVersion() {
            return Version();
        }

        LFXE_API Version UpdateManager::GetLiveVersion() {
            return Version();
        }


        LFXE_API wstring UpdateManager::GetDownloadPageUrl() {
            return nullptr;
        }

    }
}
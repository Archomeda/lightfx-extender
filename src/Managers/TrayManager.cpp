#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "TrayManager.h"


using namespace std;

namespace lightfx {
    namespace managers {

        LFXE_API TrayManager::TrayManager(const LightFXExtender* lightFXExtender) {

        }

        LFXE_API TrayManager::~TrayManager() {

        }


        LFXE_API bool TrayManager::IsAdded() {
            return nullptr;
        }

        LFXE_API void TrayManager::AddTrayIcon() {

        }

        LFXE_API void TrayManager::RemoveTrayIcon() {

        }


        //LFXE_API void TrayManager::AttachDevice(const spDevice& device) {

        //}

        //LFXE_API void TrayManager::RemoveDevice(const spDevice& device) {

        //}


        LFXE_API bool TrayManager::HasUpdateNotification() {
            return nullptr;
        }

        LFXE_API void TrayManager::SetUpdateNotification(const std::wstring& versionString, const std::wstring& downloadUrl) {

        }

    }
}
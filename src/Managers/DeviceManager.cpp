#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceManager.h"


using namespace std;
using namespace lightfx::devices;

namespace lightfx {
    namespace managers {

        LFXE_API DeviceManager::DeviceManager(const LightFXExtender* lightFXExtender) {

        }

        LFXE_API DeviceManager::~DeviceManager() {

        }


        LFXE_API void DeviceManager::AddDevice(const wstring& deviceKey, const spDevice& device) {

        }

        LFXE_API spDevice DeviceManager::GetDevice(const wstring& deviceKey) {
            return nullptr;
        }

    }
}
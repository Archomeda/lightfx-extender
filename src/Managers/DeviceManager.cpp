#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceManager.h"


using namespace std;
using namespace lightfx::devices;

namespace lightfx {
    namespace managers {

        size_t DeviceManager::InitializeDevices() {
            return 0;
        }

        size_t DeviceManager::UninitializeDevices() {
            return 0;
        }

    }
}

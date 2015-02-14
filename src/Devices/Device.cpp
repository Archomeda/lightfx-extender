#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Device.h"


using namespace std;
using namespace lightfx::managers;

namespace lightfx {
    namespace devices {

        LFXE_API Device::Device() {

        }


        LFXE_API bool Device::IsEnabled() {
            return nullptr;
        }

        LFXE_API bool Device::IsInitialized() {
            return nullptr;
        }


        LFXE_API bool Device::Enable() {
            return nullptr;
        }

        LFXE_API bool Device::Disable() {
            return nullptr;
        }


        LFXE_API bool Device::Initialize() {
            return nullptr;
        }

        LFXE_API bool Device::Release() {
            return nullptr;
        }

        LFXE_API bool Device::Update() {
            return nullptr;
        }

        LFXE_API bool Device::Reset() {
            return nullptr;
        }

    }
}
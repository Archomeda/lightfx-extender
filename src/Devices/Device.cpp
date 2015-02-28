#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Device.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/LogManager.h"


#define LOG(logLevel, line) if (this->GetManager() != nullptr) { this->GetManager()->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + line); }

using namespace std;
using namespace lightfx::managers;

namespace lightfx {
    namespace devices {

        LFXE_API Device::Device() {

        }


        LFXE_API bool Device::IsEnabled() {
            return this->isEnabled;
        }

        LFXE_API bool Device::IsInitialized() {
            return this->isInitialized;
        }


        LFXE_API bool Device::Enable() {
            LOG(LogLevel::Info, L"Enabling");
            this->isEnabled = true;
            return true;
        }

        LFXE_API bool Device::Disable() {
            LOG(LogLevel::Info, L"Disabling");
            this->isEnabled = false;
            return true;
        }


        LFXE_API bool Device::Initialize() {
            LOG(LogLevel::Info, L"Initializing");
            this->isInitialized = true;
            return true;
        }

        LFXE_API bool Device::Release() {
            LOG(LogLevel::Info, L"Releasing");
            this->isInitialized = false;
            return true;
        }

        LFXE_API bool Device::Update() {
            return nullptr;
        }

        LFXE_API bool Device::Reset() {
            return nullptr;
        }

    }
}

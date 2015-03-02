#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Game.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/LogManager.h"


#define LOG(logLevel, line) if (this->GetManager() != nullptr) { this->GetManager()->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"Game ") + this->GetGameName() + L" - " + line); }

using namespace std;
using namespace lightfx::managers;

namespace lightfx {
    namespace games {

        LFXE_API bool Game::IsEnabled() {
            return this->isEnabled;
        }

        LFXE_API bool Game::IsInitialized() {
            return this->isInitialized;
        }


        LFXE_API bool Game::Enable() {
            if (!this->isEnabled) {
                LOG(LogLevel::Info, L"Enabling");
                this->isEnabled = true;
            }
            return true;
        }

        LFXE_API bool Game::Disable() {
            if (this->isEnabled) {
                LOG(LogLevel::Info, L"Disabling");
                this->isEnabled = false;
            }
            return true;
        }

        LFXE_API bool Game::Initialize() {
            if (!this->isInitialized) {
                LOG(LogLevel::Info, L"Initializing");
                this->isInitialized = true;
            }
            return true;
        }

        LFXE_API bool Game::Release() {
            if (this->isInitialized) {
                LOG(LogLevel::Info, L"Releasing");
                this->isInitialized = false;
            }
            return true;
        }

    }
}

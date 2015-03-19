#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Game.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"Game ") + this->GetGameName() + L" - " + message)

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::utils;

namespace lightfx {
    namespace games {

        LFXE_API bool Game::IsInitialized() {
            return this->isInitialized;
        }


        LFXE_API bool Game::Initialize() {
            if (!this->isInitialized) {
                LOG(LogLevel::Debug, L"Initializing");
                this->isInitialized = true;
            }
            return true;
        }

        LFXE_API bool Game::Release() {
            if (this->isInitialized) {
                LOG(LogLevel::Debug, L"Releasing");
                this->isInitialized = false;
            }
            return true;
        }

    }
}

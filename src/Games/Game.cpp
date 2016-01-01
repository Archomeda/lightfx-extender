#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Game.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"


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
                LOG_DEBUG(L"Initializing " + this->GetGameName());
                this->isInitialized = true;
            }
            return true;
        }

        LFXE_API bool Game::Release() {
            if (this->isInitialized) {
                LOG_DEBUG(L"Releasing " + this->GetGameName());
                this->isInitialized = false;
            }
            return true;
        }

    }
}

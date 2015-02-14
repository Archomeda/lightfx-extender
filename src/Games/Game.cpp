#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Game.h"


using namespace std;

namespace lightfx {
    namespace games {

        LFXE_API Game::Game() {

        }


        LFXE_API bool Game::IsEnabled() {
            return nullptr;
        }

        LFXE_API bool Game::IsInitialized() {
            return nullptr;
        }


        LFXE_API bool Game::Initialize() {
            return nullptr;
        }

        LFXE_API bool Game::Release() {
            return nullptr;
        }

    }
}

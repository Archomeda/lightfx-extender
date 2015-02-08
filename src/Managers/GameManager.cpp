#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "GameManager.h"


using namespace std;
using namespace lightfx::games;

namespace lightfx {
    namespace managers {

        LFXE_API GameManager::GameManager(const LightFXExtender* lightFXExtender) {

        }

        LFXE_API GameManager::~GameManager() {

        }


        LFXE_API void GameManager::AddGame(const wstring& gameKey, const spGame& game) {

        }

        LFXE_API spGame GetGame(const wstring& gameKey) {
            return nullptr;
        }

    }
}
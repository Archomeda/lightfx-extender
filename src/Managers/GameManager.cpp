#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "GameManager.h"

// Project includes
#include "../Games/GameGw2.h"


using namespace std;
using namespace lightfx::games;

namespace lightfx {
    namespace managers {

        size_t GameManager::InitializeGames() {
            size_t i = 0;

            auto guildWars2 = make_shared<GameGw2>();
            if (guildWars2->Initialize()) {
                ++i;
                this->AddChild(L"Guild Wars 2", guildWars2);
            }
            
            return i;
        }

        size_t GameManager::UninitializeGames() {
            size_t i = 0;

            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                if (this->GetChildByIndex(j)->Release()) {
                    ++i;
                }
            }

            return i;
        }

    }
}

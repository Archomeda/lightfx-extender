#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "GameManager.h"

// Project includes
#include "../Games/GameGw2.h"
#include "../LightFXExtender.h"
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::games;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        bool GameManager::InitializeGame(const std::wstring& fileName) {
            LOG_DEBUG(L"Initializing special game support");

            // Guild Wars 2
            auto guildWars2 = make_shared<GameGw2>();
            if (guildWars2->GetFileName() == fileName) {
                this->AddChild(guildWars2->GetGameName(), guildWars2);
                if (guildWars2->Initialize()) {
                    LOG_INFO(L"Guild Wars 2 found");
                    return true;
                }
            }

            LOG_INFO(L"No special game support found");
            return false;
        }

        size_t GameManager::UninitializeGames() {
            size_t i = 0;

            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                if (this->GetChildByIndex(j)->Release()) {
                    ++i;
                }
            }

            LOG_INFO(L"Successfully uninitialized " + to_wstring(i) + L" games");
            return i;
        }

    }
}

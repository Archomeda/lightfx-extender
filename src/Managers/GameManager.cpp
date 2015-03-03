#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "GameManager.h"

// Project includes
#include "../Games/GameGw2.h"
#include "../LightFXExtender.h"
#include "../Managers/LogManager.h"


#define LOG(logLevel, line) if (this->GetLightFXExtender() != nullptr) { this->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"GameManager - ") + line); }

using namespace std;
using namespace lightfx::games;

namespace lightfx {
    namespace managers {

        bool GameManager::InitializeGame(const std::wstring& fileName) {
            LOG(LogLevel::Info, L"Initializing special game support");

            // Guild Wars 2
            auto guildWars2 = make_shared<GameGw2>();
            if (guildWars2->GetFileName() == fileName) {
                this->AddChild(guildWars2->GetGameName(), guildWars2);
                if (guildWars2->Initialize()) {
                    LOG(LogLevel::Info, L"Guild Wars 2 found");
                    return true;
                }
            }

            LOG(LogLevel::Info, L"No special game support found");
            return false;
        }

        size_t GameManager::UninitializeGames() {
            size_t i = 0;

            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                if (this->GetChildByIndex(j)->Release()) {
                    ++i;
                }
            }

            LOG(LogLevel::Info, L"Successfully uninitialized " + to_wstring(i) + L" games");
            return i;
        }

    }
}

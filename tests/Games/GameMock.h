#pragma once

// Standard includes
#include <string>

// Project includes
#include "../../src/Games/Game.h"


namespace lightfx_tests {
    namespace games {

        class GameMock : public lightfx::games::Game {

        public:
            using Game::IsEnabled;
            using Game::IsInitialized;

            using Game::Enable;
            using Game::Disable;

            using Game::Initialize;
            using Game::Release;

            virtual const std::wstring GetGameName() override { return L"GameName"; }
            virtual const std::wstring GetFileName() override { return L"FileName"; }

        };

    }
}

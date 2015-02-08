#pragma once

// Standard includes
#include <string>
#include <map>
#include <memory>

// Project includes
#include "../LightFXExtender.h"
#include "../Games/Game.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    class LightFXExtender;

    namespace managers {

        typedef std::shared_ptr<games::Game> spGame;
        typedef std::map<std::wstring, spGame> mapGames;

        class LFXE_API GameManager {

        public:
            GameManager(const LightFXExtender* lightFXExtender);
            ~GameManager();

            void AddGame(const std::wstring& deviceKey, const spGame& device);
            spGame GetGame(const std::wstring& deviceKey);

#pragma warning(push)
#pragma warning(disable : 4251)

        private:
            mapGames games = {};

#pragma warning(pop)

        };

    }
}

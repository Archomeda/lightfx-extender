#pragma once

// Project includes
#include "Manager.h"
#include "../Games/Game.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace games {
        class Game;
    }

    namespace managers {

        class LFXE_API GameManager : public Manager < games::Game, GameManager > {

        public:
            GameManager() {}

        };

        template class LFXE_API Manager < games::Game, GameManager >;

    }
}

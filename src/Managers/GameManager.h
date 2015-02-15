#pragma once

// Project includes
#include "ManagerWithChildren.h"
#include "../Games/Game.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace games {
        class Game;
    }

    namespace managers {

        class LFXE_API GameManager : public ManagerWithChildren < games::Game, GameManager > {

        public:
            GameManager() {}

        };

    }
}

#pragma warning(pop)

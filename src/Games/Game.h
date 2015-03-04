#pragma once

// Standard includes
#include <string>

// Project includes
#include "../Managers/GameManager.h"
#include "../Managers/ChildOfManager.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class GameManager;
    }

    namespace games {

        class LFXE_API Game : public managers::ChildOfManager < managers::GameManager > {

        public:
            Game() {}
            virtual ~Game() {}

            bool IsInitialized();

            virtual bool Initialize();
            virtual bool Release();

            virtual const std::wstring GetGameName() = 0;
            virtual const std::wstring GetFileName() = 0;

        private:
            bool isEnabled = false;
            bool isInitialized = false;

        };

    }
}

#pragma warning(pop)

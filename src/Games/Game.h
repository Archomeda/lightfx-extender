#pragma once

// Standard includes
#include <string>

// Project includes
#include "../Managers/GameManager.h"
#include "../Managers/ChildOfManager.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace managers {
        class GameManager;
    }

    namespace games {

        class LFXE_API Game : public managers::ChildOfManager < managers::GameManager > {

        public:
            Game();

            bool IsEnabled();
            bool IsInitialized();

            virtual bool Initialize();
            virtual bool Release();

            virtual std::wstring GetGameName() = 0;
            virtual unsigned char GetFileName() = 0;

        private:
            bool isEnabled = false;
            bool isInitialized = false;

        };

    }
}

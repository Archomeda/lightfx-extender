#pragma once

// Standard includes
#include <string>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace games {

        class LFXE_API Game {

        public:
            Game();
            ~Game();

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

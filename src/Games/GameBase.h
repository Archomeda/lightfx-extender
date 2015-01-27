#pragma once

// Standard includes
#include <string>


namespace lightfx {
    namespace games {

        class GameBase {

        public:
            GameBase() {};

            virtual void Initialize() = 0;
            virtual void Release() = 0;

            virtual std::wstring GetName() = 0;
            virtual std::wstring GetFileName() = 0;

        };

    }
}

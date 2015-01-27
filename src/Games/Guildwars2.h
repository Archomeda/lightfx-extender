#pragma once

// Standard includes
#include <string>
#include <thread>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>

// Project includes
#include "GameBase.h"
#include "MumbleLink.h"


namespace lightfx {
    namespace games {

        class Guildwars2 : public GameBase {

        public:
            virtual void Initialize() override;
            virtual void Release() override;

            virtual std::wstring GetName() override;
            virtual std::wstring GetFileName() override;

        protected:
            void UpdateLightsWithTeamColor(const unsigned int teamColorId);


        private:
            unsigned long lastTeamColorLightUpdate = 0;
            unsigned int lastTeamColorId = 0;
            bool mumbleLinkLoopRunning = false;

            HANDLE hMapObject = NULL;
            std::unique_ptr<MumbleLinkedMem> mumbleLinkedMem;

            std::thread mumbleLinkLoopThread;
            unsigned int lastTick;
            void MumbleLinkLoop();

        };

    }
}

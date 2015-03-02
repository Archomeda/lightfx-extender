#pragma once

// Standard includes
#include <string>
#include <thread>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>

// Project includes
#include "Game.h"
#include "MumbleLink.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace games {

        class LFXE_API GameGw2 : public Game {

        public:
            virtual bool Initialize() override;
            virtual bool Release() override;

            virtual const std::wstring GetGameName() override { return L"Guild Wars 2"; }
            virtual const std::wstring GetFileName() override { return L"Gw2.exe"; }

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

#pragma warning(pop)

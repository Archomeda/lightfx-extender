#include "Guildwars2.h"

// Standard includes
#include <chrono>

// 3rd party includes
#include "rapidjson/document.h"

// Project includes
#include "../LightsManager.h"
#include "../Utils/Log.h"


using namespace std;
using namespace rapidjson;
using namespace lightfx::utils;

#define GAMENAME L"Guild Wars 2"
#define GAMEFILENAME L"Gw2.exe"

namespace lightfx {
    namespace games {

        void Guildwars2::Initialize() {
            this->hMapObject = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(MumbleLinkedMem), L"MumbleLink");
            if (this->hMapObject == NULL) {
                return;
            }

            this->mumbleLinkedMem = unique_ptr<MumbleLinkedMem>((MumbleLinkedMem*)MapViewOfFile(this->hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MumbleLinkedMem)));
            if (!this->mumbleLinkedMem) {
                this->Release();
                return;
            }

            this->mumbleLinkLoopRunning = true;
            this->mumbleLinkLoopThread = thread(&Guildwars2::MumbleLinkLoop, this);
        }

        void Guildwars2::Release() {
            this->mumbleLinkLoopRunning = false;
            this->mumbleLinkLoopThread.join();

            if (this->mumbleLinkedMem) {
                UnmapViewOfFile(this->mumbleLinkedMem.get());
                this->mumbleLinkedMem.release();
            }

            if (this->hMapObject != NULL) {
                CloseHandle(this->hMapObject);
                this->hMapObject = NULL;
            }
        }

        wstring Guildwars2::GetName() {
            return GAMENAME;
        }

        wstring Guildwars2::GetFileName() {
            return GAMEFILENAME;
        }

        void Guildwars2::UpdateLightsWithTeamColor(const unsigned int teamColorId) {
            unsigned long currentTick = GetTickCount();
            if (teamColorId == 0) {
                // Not in a team
                this->lastTeamColorLightUpdate = 0;
            } else if (this->lastTeamColorLightUpdate == 0 || this->lastTeamColorId != teamColorId) {
                // No update done so far, or player switched teams.
                // Assume this is the start of the match (PvP) or map join (WvW).
                // Therefore we can show the team color.

                LFX_COLOR color = {};
                color.brightness = 255;
                switch (teamColorId) {
                case 9: // Blue
                    color.blue = 255;
                    Log(this->GetName() + L": Detected PvP/WvW blue team");
                    break;
                case 55: // Green
                    color.green = 255;
                    Log(this->GetName() + L": Detected PvP/WvW green team");
                    break;
                case 376: // Red
                    color.red = 255;
                    Log(this->GetName() + L": Detected PvP/WvW red team");
                    break;
                }
                LightsManager::Instance().Pulse(color, 1000, 5);
                LightsManager::Instance().Update();
                this->lastTeamColorLightUpdate = currentTick;
            }

            this->lastTeamColorId = teamColorId;
        }

        void Guildwars2::MumbleLinkLoop() {
            Log(this->GetName() + L": Mumble Link started");

            while (this->mumbleLinkLoopRunning) {
                if (this->lastTick != this->mumbleLinkedMem->uiTick) {
                    this->lastTick = this->mumbleLinkedMem->uiTick;
                    wstring name = wstring(this->mumbleLinkedMem->name);
                    if (name == GAMENAME) {
                        wchar_t* identity = this->mumbleLinkedMem->identity;

                        try {
                            GenericDocument<UTF16<>> doc;
                            doc.Parse<0>(identity);

                            // Check the team color
                            if (doc.HasMember(L"team_color_id") && doc[L"team_color_id"].IsUint()) {
                                this->UpdateLightsWithTeamColor(doc[L"team_color_id"].GetUint());
                            }
                        } catch (...) {}
                    }
                }

                this_thread::sleep_for(chrono::milliseconds(20));
            }
        }


    }
}

#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "GameGw2.h"

// Standard includes
#include <chrono>

// 3rd party includes
#include "rapidjson/document.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Config/MainConfigFile.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/DeviceManager.h"
#include "../Utils/Log.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"Game ") + this->GetGameName() + L" - " + message)

using namespace std;
using namespace rapidjson;
using namespace lightfx::devices;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
    namespace games {

        LFXE_API bool GameGw2::Initialize() {
            if (!this->IsInitialized()) {
                if (Game::Initialize()) {
                    this->hMapObject = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(MumbleLinkedMem), L"MumbleLink");
                    if (this->hMapObject == NULL) {
                        return false;
                    }

                    this->mumbleLinkedMem = unique_ptr<MumbleLinkedMem>((MumbleLinkedMem*)MapViewOfFile(this->hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MumbleLinkedMem)));
                    if (!this->mumbleLinkedMem) {
                        this->Release();
                        return false;
                    }

                    this->mumbleLinkLoopRunning = true;
                    this->mumbleLinkLoopThread = thread(&GameGw2::MumbleLinkLoop, this);
                    return true;
                }
            }
            return false;
        }

        LFXE_API bool GameGw2::Release() {
            if (this->IsInitialized()) {
                if (Game::Release()) {
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
                    return true;
                }
            }
            return false;
        }

        LFXE_API void GameGw2::UpdateLightsWithTeamColor(const unsigned int teamColorId) {
            unsigned long currentTick = GetTickCount();
            if (teamColorId == 0) {
                // Not in a team
                this->lastTeamColorLightUpdate = 0;
            } else if (this->lastTeamColorLightUpdate == 0 || this->lastTeamColorId != teamColorId) {
                // No update done so far, or player switched teams.
                // Assume this is the start of the match (PvP) or map join (WvW).
                // Therefore we can show the team color.

                LightColor startColor(0, 0, 0, 0);
                LightColor endColor(0, 0, 0, 255);
                switch (teamColorId) {
                case 9: // Blue
                    startColor.blue = 255;
                    endColor.blue = 255;
                    LOG(LogLevel::Debug, L"Mumble Link - Detected PvP/WvW blue team");
                    break;
                case 55: // Green
                    startColor.green = 255;
                    endColor.green = 255;
                    LOG(LogLevel::Debug, L"Mumble Link - Detected PvP/WvW green team");
                    break;
                case 376: // Red
                    startColor.red = 255;
                    endColor.red = 255;
                    LOG(LogLevel::Debug, L"Mumble Link - Detected PvP/WvW red team");
                    break;
                }

                auto deviceManager = this->GetManager()->GetLightFXExtender()->GetDeviceManager();
                for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                    auto device = deviceManager->GetChildByIndex(i);
                    vector<LightColor> resetColor = device->GetRecentTimeline().GetColorAtTime(device->GetRecentTimeline().GetTotalDuration());

                    Timeline timeline;
                    auto config = this->GetManager()->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
                    if (config->GuildWars2TeamColorAnimation == L"Pulse") {
                        vector<LightTimeline> timelines;
                        for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                            timelines.push_back(LightTimeline::NewPulse(startColor, endColor, resetColor[j], 200, 5, 100, 400));
                        }
                        timeline = Timeline(timelines);
                    } else if (config->GuildWars2TeamColorAnimation == L"Walk") {
                        timeline = Timeline::NewWalk(device->GetNumberOfLights(), startColor, endColor, resetColor, 1000, 4, 100);
                    }
                    device->QueueTimeline(timeline);
                    device->Update();
                }

                this->lastTeamColorLightUpdate = currentTick;
            }

            this->lastTeamColorId = teamColorId;
        }

        LFXE_API void GameGw2::MumbleLinkLoop() {
            LOG(LogLevel::Debug, L"Mumble Link started");

            while (this->mumbleLinkLoopRunning) {
                if (this->lastTick != this->mumbleLinkedMem->uiTick) {
                    this->lastTick = this->mumbleLinkedMem->uiTick;
                    wstring name = wstring(this->mumbleLinkedMem->name);
                    if (name == this->GetGameName()) {
                        wchar_t* identity = this->mumbleLinkedMem->identity;

                        try {
                            GenericDocument<UTF16<>> doc;
                            doc.Parse<0>(identity);

                            // Check the team color (if setting is enabled)
                            auto config = this->GetManager()->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
                            if (config->GuildWars2TeamColorEnabled) {
                                if (doc.HasMember(L"team_color_id") && doc[L"team_color_id"].IsUint()) {
                                    this->UpdateLightsWithTeamColor(doc[L"team_color_id"].GetUint());
                                }
                            }
                        } catch (...) {
                            LOG(LogLevel::Error, L"Error when parsing Mumble Link identity: " + wstring(identity));
                        }
                    }
                }

                this_thread::sleep_for(chrono::milliseconds(20)); // Update rate: 50/s
            }
        }


    }
}

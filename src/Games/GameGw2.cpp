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
#include "../Managers/VendorManager.h"
#include "../Utils/Log.h"


using namespace std;
using namespace rapidjson;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;
using namespace lightfx::vendors;

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

                LightColor startColor;
                LightColor endColor;
                switch (teamColorId) {
                case 9: // Blue
                    startColor = { 29, 118, 169, 0 };
                    endColor = { 29, 118, 169, 255 };
                    LOG_DEBUG(L"Mumble Link - Detected PvP/WvW blue team");
                    break;
                case 55: // Green
                    startColor = { 119, 192, 33, 0 };
                    endColor = { 119, 192, 33, 255 };
                    LOG_DEBUG(L"Mumble Link - Detected PvP/WvW green team");
                    break;
                case 376: // Red
                    startColor = { 203, 28, 36, 0 };
                    endColor = { 203, 28, 36, 255 };
                    LOG_DEBUG(L"Mumble Link - Detected PvP/WvW red team");
                    break;
                default: // Unknown
                    LOG_INFO(L"Mumble Link - Detected unknown team color: " + to_wstring(teamColorId) + L", skipping");
                    return;
                }

                auto vendorManager = this->GetManager()->GetLightFXExtender()->GetVendorManager();
                for (size_t i = 0; i < vendorManager->GetChildrenCount(); ++i) {
                    auto device = vendorManager->GetChildByIndex(i);
                    if (!device->IsEnabled()) {
                        continue;
                    }

                    Timeline activeTimeline = device->GetActiveTimeline();
                    vector<LightColor> resetColor = activeTimeline.GetColorAtTime(activeTimeline.GetTotalDuration());

                    Timeline timeline;
                    auto config = this->GetManager()->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
                    if (config->GuildWars2TeamColorAnimation == L"Pulse") {
                        vector<LightTimeline> timelines;
                        for (size_t j = 0; j < device->GetNumberOfLights(); ++j) {
                            LightColor color(255, 255, 255, 255);
                            if (j < resetColor.size()) {
                                color = resetColor[j];
                            } else {
                                LOG_WARNING(L"No reset color defined for pulsing team color, using fallback color white");
                            }
                            timelines.push_back(LightTimeline::NewPulse(startColor, endColor, color, 200, 5, 100, 400));
                        }
                        timeline = Timeline(timelines);
                    } else if (config->GuildWars2TeamColorAnimation == L"Walk") {
                        timeline = Timeline::NewWalk(device->GetNumberOfLights(), startColor, endColor, resetColor, 1000, 4, 100);
                    }
                    device->SetQueuedTimeline(timeline);
                }
                vendorManager->UpdateLights();

                this->lastTeamColorLightUpdate = currentTick;
            }

            this->lastTeamColorId = teamColorId;
        }

        LFXE_API void GameGw2::MumbleLinkLoop() {
            LOG_DEBUG(L"Mumble Link started");

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
                            LOG_ERROR(L"Error when parsing Mumble Link identity: " + wstring(identity));
                        }
                    }
                }

                this_thread::sleep_for(chrono::milliseconds(20)); // Update rate: 50/s
            }
        }


    }
}

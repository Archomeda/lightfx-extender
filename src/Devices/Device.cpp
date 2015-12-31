#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Device.h"

// Standard includes
#include <chrono>

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/ConfigManager.h"
#include "../Config/MainConfigFile.h"
#include "../Utils/Log.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + message)

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
    namespace devices {

        LFXE_API Device::~Device() {
            this->Release();
        }


        LFXE_API bool Device::IsEnabled() {
            return this->isEnabled;
        }

        LFXE_API bool Device::IsInitialized() {
            return this->isInitialized;
        }


        LFXE_API bool Device::Enable() {
            if (!this->isEnabled) {
                LOG(LogLevel::Debug, L"Enabling");
                this->isEnabled = true;
            }
            return true;
        }

        LFXE_API bool Device::Disable() {
            if (this->isEnabled) {
                LOG(LogLevel::Debug, L"Disabling");
                this->isEnabled = false;
            }
            return true;
        }


        LFXE_API bool Device::Initialize() {
            if (!this->isInitialized) {
                LOG(LogLevel::Debug, L"Initializing");
                this->Reset();
                this->isInitialized = true;
            }
            return true;
        }

        LFXE_API bool Device::Release() {
            if (this->isInitialized) {
                this->Disable();
                LOG(LogLevel::Debug, L"Releasing");
                this->isInitialized = false;
            }
            return true;
        }

        LFXE_API bool Device::Update(const chrono::milliseconds& timeTick) {
            if (this->isEnabled) {
                if (this->timelineDone) {
                    // Previous timeline is done, get the next queued one if there is one
                    lock_guard<mutex> lock(this->TimelineQueueMutex);
                    if (this->TimelineQueue.size() > 0) {
                        this->ActiveTimeline = this->TimelineQueue.front();
                        this->TimelineQueue.pop();
                        this->timelineStart = timeTick;
                        LOG(LogLevel::Debug, L"Performing timeline " + this->ActiveTimeline.ToString());
                    } else {
                        return true;
                    }
                }

                // Get the color at the current time
                unsigned long timelineTick = (timeTick - this->timelineStart).count();                
                vector<LightColor> newColor = this->ActiveTimeline.GetColorAtTime(timelineTick);
                if (newColor.size() == 0) {
                    LOG(LogLevel::Warning, L"Timeline has no colors set! Ignoring update");
                    return true;
                }
                bool needsUpdate = this->lightColor.size() != newColor.size();
                if (!needsUpdate) {
                    for (size_t i = 0; i < newColor.size(); ++i) {
                        if (this->lightColor[i] != newColor[i]) {
                            needsUpdate = true;
                            break;
                        }
                    }
                }

                // Update color if needed
                if (needsUpdate) {
                    this->lightColor = newColor;
                    this->PushColorToDevice(this->lightColor);
                }

                this->timelineDone = timelineTick >= this->ActiveTimeline.GetTotalDuration();
                return this->timelineDone && this->TimelineQueue.size() == 0;
            }
            return true;
        }

        LFXE_API bool Device::Reset() {
            this->ActiveTimeline = {};
            this->QueuedTimeline = {};
            this->TimelineQueue = {};

            this->timelineDone = true;
            this->timelineStart = {};

            return true;
        }


        LFXE_API Timeline Device::GetActiveTimeline() {
            return this->ActiveTimeline;
        }

        LFXE_API Timeline Device::GetQueuedTimeline() {
            lock_guard<mutex> lock(this->QueuedTimelineMutex);
            return this->QueuedTimeline;
        }

        LFXE_API Timeline Device::GetRecentTimeline() {
            if (!this->TimelineQueue.empty()) {
                return this->TimelineQueue.back();
            }
            return this->ActiveTimeline;
        }

        LFXE_API void Device::QueueTimeline(const Timeline& timeline) {
            lock_guard<mutex> lock(this->QueuedTimelineMutex);
            this->QueuedTimeline = timeline;
        }

        LFXE_API bool Device::CommitQueuedTimeline(const bool flushQueue) {
            if (this->isEnabled) {
                // Place the pre-queued item into the queue
                lock_guard<mutex> lock(this->TimelineQueueMutex);
                if (flushQueue) {
                    this->TimelineQueue = {};
                    this->timelineDone = true;
                }
                lock_guard<mutex> lock2(this->QueuedTimelineMutex);
                this->TimelineQueue.push(this->QueuedTimeline);
                this->QueuedTimeline = {};
                return true;
            }
            return false;
        }


        LFXE_API const size_t Device::GetNumberOfLights() {
            return this->numberOfLights;
        }

        LFXE_API void Device::SetNumberOfLights(const size_t numberOfLights) {
            this->numberOfLights = numberOfLights;
            this->lightData.resize(numberOfLights);
        }

        LFXE_API LightColor Device::GetLightColor(const size_t lightIndex) {
            if (this->lightColor.size() > lightIndex) {
                return this->lightColor[lightIndex];
            }
            return LightColor(0, 0, 0, 0);
        }

        LFXE_API LightData Device::GetLightData(const size_t lightIndex) {
            return this->lightData[lightIndex];
        }

        LFXE_API void Device::SetLightData(const size_t lightIndex, const LightData& lightData) {
            this->lightData[lightIndex] = lightData;
        }


        LFXE_API void Device::SetEnabled(const bool enabled) {
            this->isEnabled = enabled;
        }

        LFXE_API void Device::SetInitialized(const bool initialized) {
            this->isInitialized = initialized;
        }

    }
}

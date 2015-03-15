#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Device.h"

// Standard includes
#include <chrono>

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/LogManager.h"


#define LOG(logLevel, message) if (this->GetManager() != nullptr) { LOG_(this->GetManager()->GetLightFXExtender()->GetLogManager(), logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + message) }

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::timelines;

namespace lightfx {
    namespace devices {

        LFXE_API Device::~Device() {
            this->StopUpdateWorker();
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
                this->StartUpdateWorker();
                this->isEnabled = true;
            }
            return true;
        }

        LFXE_API bool Device::Disable() {
            if (this->isEnabled) {
                LOG(LogLevel::Debug, L"Disabling");
                this->StopUpdateWorker();
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

        LFXE_API bool Device::Update(bool flushQueue) {
            if (this->isEnabled) {
                // Place the pre-queued item into the queue
                    {
                        lock_guard<mutex> lock(this->TimelineQueueMutex);
                        if (flushQueue) {
                            this->TimelineQueue = {};
                            this->TimelineQueueFlush = true;
                        }
                        this->TimelineQueue.push(this->QueuedTimeline);
                        this->QueuedTimeline = {};
                    }
                return true;
            }
            return false;
        }

        LFXE_API bool Device::Reset() {
            bool running = this->updateWorkerActive;

            if (running) {
                this->StopUpdateWorker();
            }

            this->ActiveTimeline = {};
            this->QueuedTimeline = {};
            this->TimelineQueue = {};
            this->TimelineQueueFlush = false;

            if (running) {
                this->StartUpdateWorker();
            }

            return true;
        }


        LFXE_API Timeline Device::GetActiveTimeline() {
            return this->ActiveTimeline;
        }

        LFXE_API Timeline Device::GetQueuedTimeline() {
            return this->QueuedTimeline;
        }

        LFXE_API Timeline Device::GetRecentTimeline() {
            if (!this->TimelineQueue.empty()) {
                return this->TimelineQueue.back();
            }
            return this->ActiveTimeline;
        }

        LFXE_API void Device::QueueTimeline(const Timeline& timeline) {
            this->QueuedTimeline = timeline;
        }

        LFXE_API void Device::NotifyUpdate() {
            this->notifyUpdateWorker = true;
            this->updateWorkerCv.notify_all();
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


        LFXE_API void Device::StartUpdateWorker() {
            if (!this->updateWorkerActive) {
                this->updateWorkerActive = true;
                this->stopUpdateWorker = false;
                this->updateWorkerThread = thread(&Device::UpdateWorker, this);
            }
        }

        LFXE_API void Device::StopUpdateWorker() {
            if (this->updateWorkerActive) {
                this->stopUpdateWorker = true;
                this->updateWorkerCv.notify_all();
                if (this->updateWorkerThread.joinable()) {
                    this->updateWorkerThread.join();
                }
                this->updateWorkerActive = false;
            }
        }

        LFXE_API void Device::UpdateWorker() {
            bool isUpdating = false;
            unsigned long long timelineStart = 0;

            while (!this->stopUpdateWorker) {
                unique_lock<mutex> lock(this->updateWorkerCvMutex);
                this->updateWorkerCv.wait(lock, [&] { return this->notifyUpdateWorker && (isUpdating || !this->TimelineQueue.empty()) || this->stopUpdateWorker; });
                this->notifyUpdateWorker = false;

                if (this->stopUpdateWorker) {
                    break;
                }

                if (!this->TimelineQueue.empty()) {
                    if (!isUpdating || this->TimelineQueueFlush) {
                        // Not currently updating the color or the queue needs to be flushed
                        this->TimelineQueueFlush = false;
                        isUpdating = true;
                        {
                            lock_guard<mutex> lock(this->TimelineQueueMutex);
                            this->ActiveTimeline = this->TimelineQueue.front();
                            this->TimelineQueue.pop();
                        }
                        LOG(LogLevel::Debug, L"Performing timeline " + this->ActiveTimeline.ToString());
                        timelineStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                    }
                }

                if (isUpdating) {
                    // Get the color at the current time
                    unsigned long timelinePos = unsigned long(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - timelineStart);
                    vector<LightColor> newColor = this->ActiveTimeline.GetColorAtTime(timelinePos);
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

                    isUpdating = timelinePos <= this->ActiveTimeline.GetTotalDuration();
                }
            }
        }

    }
}

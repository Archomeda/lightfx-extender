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
            this->StopLightColorUpdateWorker();
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
                this->StartLightColorUpdateWorker();
                this->isEnabled = true;
            }
            return true;
        }

        LFXE_API bool Device::Disable() {
            if (this->isEnabled) {
                LOG(LogLevel::Debug, L"Disabling");
                this->StopLightColorUpdateWorker();
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
                        lock_guard<mutex> lock1(this->TimelineQueueMutex);
                        lock_guard<mutex> lock2(this->lightColorUpdateThreadMutex);
                        if (flushQueue) {
                            this->TimelineQueue = {};
                            this->TimelineQueueFlush = true;
                        }
                        this->TimelineQueue.push(this->QueuedTimeline);
                        this->QueuedTimeline = {};
                    }

                // Signal the update thread
                this->lightColorUpdateThreadConditionVariable.notify_one();
                return true;
            }
            return false;
        }

        LFXE_API bool Device::Reset() {
            bool running = this->lightColorUpdateThreadRunning;

            if (running) {
                this->StopLightColorUpdateWorker();
            }

            this->ActiveTimeline = {};
            this->QueuedTimeline = {};
            this->TimelineQueue = {};
            this->TimelineQueueFlush = false;

            if (running) {
                this->StartLightColorUpdateWorker();
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
            return LightColor();
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


        LFXE_API void Device::StartLightColorUpdateWorker() {
            if (!this->lightColorUpdateThreadRunning) {
                // Start the update thread
                this->lightColorUpdateThreadRunning = true;
                this->lightColorUpdateThread = thread(&Device::LightColorUpdateWorkerThread, this);
            }
        }

        LFXE_API void Device::StopLightColorUpdateWorker() {
            if (this->lightColorUpdateThreadRunning) {
                // Notify the update thread to exit
                    {
                        lock_guard<mutex> lock(this->lightColorUpdateThreadMutex);
                        this->lightColorUpdateThreadRunning = false;
                    }

                this->lightColorUpdateThreadConditionVariable.notify_one();
                if (this->lightColorUpdateThread.joinable()) {
                    this->lightColorUpdateThread.join();
                }
            }
        }

        LFXE_API void Device::LightColorUpdateWorkerThread() {
            bool isUpdating = false;
            unsigned long long timelineStart = 0;

            while (this->lightColorUpdateThreadRunning) {
                if (!isUpdating || this->TimelineQueueFlush) {
                    if (this->TimelineQueue.empty()) {
                        // Only wait for an update if we are not currently busy updating the colors and there's nothing in the queue
                        unique_lock<mutex> lock(this->lightColorUpdateThreadMutex);
                        this->lightColorUpdateThreadConditionVariable.wait(lock, [&] { return !this->TimelineQueue.empty() || !this->lightColorUpdateThreadRunning; });

                        if (!this->lightColorUpdateThreadRunning) {
                            // Make sure to exit the loop before trying to update the colors if the thread should be exited
                            break;
                        }
                    }

                    {
                        lock_guard<mutex> lock(this->TimelineQueueMutex);
                        this->TimelineQueueFlush = false;
                        this->ActiveTimeline = this->TimelineQueue.front();
                        this->TimelineQueue.pop();
                    }

                    LOG(LogLevel::Debug, L"Performing timeline " + this->ActiveTimeline.ToString());
                    this->lightColor = this->ActiveTimeline.GetColorAtTime(0);
                    this->PushColorToDevice(this->lightColor);

                    isUpdating = true;
                    timelineStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                } else if (isUpdating) {
                    unsigned long timelinePos = unsigned long(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - timelineStart);
                    vector<LightColor> newColor = this->ActiveTimeline.GetColorAtTime(timelinePos);
                    bool needsUpdate = false;
                    for (size_t i = 0; i < newColor.size(); ++i) {
                        if (this->lightColor[i] != newColor[i]) {
                            needsUpdate = true;
                            break;
                        }
                    }

                    if (needsUpdate) {
                        this->lightColor = newColor;
                        this->PushColorToDevice(this->lightColor);
                    }

                    isUpdating = timelinePos <= this->ActiveTimeline.GetTotalDuration();
                }

                try {
                    this_thread::sleep_for(chrono::milliseconds(5));
                } catch (...) {
                    return;
                }
            }
        }

    }
}

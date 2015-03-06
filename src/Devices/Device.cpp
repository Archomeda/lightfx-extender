#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Device.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/LogManager.h"


#define LOG(logLevel, line) if (this->GetManager() != nullptr) { this->GetManager()->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + line); }

using namespace std;
using namespace lightfx::managers;

namespace lightfx {
    namespace devices {

        LFXE_API Device::~Device() {
            this->StopUpdateCurrentColorWorker();
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
                this->StartUpdateCurrentColorWorker();
                this->isEnabled = true;
            }
            return true;
        }

        LFXE_API bool Device::Disable() {
            if (this->isEnabled) {
                LOG(LogLevel::Debug, L"Disabling");
                this->StopUpdateCurrentColorWorker();
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
                        lock_guard<mutex> lock1(this->LightActionQueueMutex);
                        lock_guard<mutex> lock2(this->lightActionUpdateThreadMutex);
                        if (flushQueue) {
                            this->LightActionQueue = {};
                            this->LightActionQueueFlush = true;
                        }
                        this->LightActionQueue.push(this->QueuedLightAction);
                        this->QueuedLightAction = {};
                    }

                // Signal the update thread
                this->lightActionUpdateThreadConditionVariable.notify_one();
                return true;
            }
            return false;
        }

        LFXE_API bool Device::Reset() {
            if (this->lightActionUpdateThreadRunning) {
                this->StopUpdateCurrentColorWorker();
                this->ActiveLightAction = LightAction(this->GetNumberOfLights());
                this->QueuedLightAction = {};
                this->LightActionQueue = {};
                this->StartUpdateCurrentColorWorker();
            } else {
                this->ActiveLightAction = LightAction(this->GetNumberOfLights());
                this->QueuedLightAction = {};
                this->LightActionQueue = {};
            }
            return true;
        }


        LFXE_API LightAction Device::GetActiveLightAction() {
            return this->ActiveLightAction;
        }

        LFXE_API LightAction Device::GetQueuedLightAction() {
            return this->QueuedLightAction;
        }

        LFXE_API LightAction Device::GetLastLightAction() {
            if (!this->LightActionQueue.empty()) {
                return this->LightActionQueue.back();
            }
            return this->ActiveLightAction;
        }

        LFXE_API void Device::QueueLightAction(const LightAction& lightAction) {
            this->QueuedLightAction = lightAction;
        }


        LFXE_API const size_t Device::GetNumberOfLights() {
            return this->numberOfLights;
        }

        LFXE_API void Device::SetNumberOfLights(const size_t numberOfLights) {
            this->numberOfLights = numberOfLights;
            this->lightData.resize(numberOfLights);
        }

        LFXE_API LightData Device::GetLightData(const size_t lightIndex) {
            return this->lightData[lightIndex];
        }

        LFXE_API void Device::SetLightData(const size_t lightIndex, const LightData& lightData) {
            this->lightData[lightIndex] = lightData;
        }


        LFXE_API void Device::StartUpdateCurrentColorWorker() {
            if (!this->lightActionUpdateThreadRunning) {
                // Start the update thread
                this->lightActionUpdateThreadRunning = true;
                this->lightActionUpdateThread = thread(&Device::UpdateCurrentColorWorkerThread, this);
            }
        }

        LFXE_API void Device::StopUpdateCurrentColorWorker() {
            if (this->lightActionUpdateThreadRunning) {
                // Notify the update thread to exit
                    {
                        lock_guard<mutex> lock(this->lightActionUpdateThreadMutex);
                        this->lightActionUpdateThreadRunning = false;
                    }

                this->lightActionUpdateThreadConditionVariable.notify_one();
                if (this->lightActionUpdateThread.joinable()) {
                    this->lightActionUpdateThread.join();
                }
            }
        }

        LFXE_API void Device::UpdateCurrentColorWorkerThread() {
            bool isUpdating = false;

            while (this->lightActionUpdateThreadRunning) {
                if (!isUpdating || this->LightActionQueueFlush) {
                    if (this->LightActionQueue.empty()) {
                        // Only wait for an update if we are not currently busy updating the colors and there's nothing in the queue
                        unique_lock<mutex> lock(this->lightActionUpdateThreadMutex);
                        this->lightActionUpdateThreadConditionVariable.wait(lock, [&] { return !this->LightActionQueue.empty() || !this->lightActionUpdateThreadRunning; });

                        if (!this->lightActionUpdateThreadRunning) {
                            // Make sure to exit the loop before trying to update the colors if the thread should be exited
                            break;
                        }
                    }

                    {
                        lock_guard<mutex> lock(this->LightActionQueueMutex);
                        this->LightActionQueueFlush = false;
                        this->ActiveLightAction = this->LightActionQueue.front();
                        this->LightActionQueue.pop();
                    }
                }

                // Update color if possible
                if (this->ActiveLightAction.CanUpdateCurrentColor()) {
                    if (this->ActiveLightAction.UpdateCurrentColor()) {
                        this->PushColorToDevice();
                    }
                    isUpdating = true;
                } else {
                    isUpdating = false;
                }

                try {
                    this_thread::sleep_for(chrono::milliseconds(1));
                } catch (...) {
                    return;
                }
            }
        }

    }
}

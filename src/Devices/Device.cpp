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
            this->StopUpdateCurrentColor();
        }


        LFXE_API bool Device::IsEnabled() {
            return this->isEnabled;
        }

        LFXE_API bool Device::IsInitialized() {
            return this->isInitialized;
        }


        LFXE_API bool Device::Enable() {
            LOG(LogLevel::Info, L"Enabling");
            this->isEnabled = true;
            return true;
        }

        LFXE_API bool Device::Disable() {
            LOG(LogLevel::Info, L"Disabling");
            this->StopUpdateCurrentColor();
            this->isEnabled = false;
            return true;
        }


        LFXE_API bool Device::Initialize() {
            LOG(LogLevel::Info, L"Initializing");
            this->Reset();
            this->isInitialized = true;
            return true;
        }

        LFXE_API bool Device::Release() {
            LOG(LogLevel::Info, L"Releasing");
            this->isInitialized = false;
            return true;
        }

        LFXE_API bool Device::Update() {
            this->CurrentLightAction = LightAction(this->QueuedLightAction);
            this->StopUpdateCurrentColor();
            this->StartUpdateCurrentColor();
            return true;
        }

        LFXE_API bool Device::Reset() {
            this->CurrentLightAction = LightAction(LightActionType::Instant, this->GetNumberOfLights(), LightColor(0, 0, 0, 0), LightColor(0, 0, 0, 0), 200, 0, 0, 5);
            this->QueuedLightAction = LightAction(this->CurrentLightAction);
            return true;
        }

        LFXE_API LightAction Device::GetCurrentLightAction() {
            return this->CurrentLightAction;
        }

        LFXE_API LightAction Device::GetQueuedLightAction() {
            return this->QueuedLightAction;
        }

        LFXE_API void Device::QueueLightAction(const LightAction& lightAction) {
            this->QueuedLightAction = lightAction;
        }


        LFXE_API void Device::UpdateCurrentColorLoop() {
            while (true) {
                this->lightActionUpdateThreadRunningMutex.lock();
                bool isRunning = this->lightActionUpdateThreadRunning;
                this->lightActionUpdateThreadRunningMutex.unlock();

                if (!isRunning) {
                    break;
                }

                if (this->CurrentLightAction.CanUpdateCurrentColor()) {
                    if (this->CurrentLightAction.UpdateCurrentColor()) {
                        this->PushColorToDevice();
                    }
                } else {
                    // Finished updating
                    break;
                }

                this_thread::sleep_for(chrono::milliseconds(5));
            }

            this->lightActionUpdateThreadRunningMutex.lock();
            this->lightActionUpdateThreadRunning = false;
            this->lightActionUpdateThreadRunningMutex.unlock();
        }

        LFXE_API void Device::StartUpdateCurrentColor() {
            this->lightActionUpdateThreadRunningMutex.lock();
            bool isRunning = this->lightActionUpdateThreadRunning;
            this->lightActionUpdateThreadRunningMutex.unlock();
            
            if (!isRunning) {
                this->lightActionUpdateThreadRunning = true;
                this->lightActionUpdateThread = thread(&Device::UpdateCurrentColorLoop, this);
            }
        }

        LFXE_API void Device::StopUpdateCurrentColor() {
            this->lightActionUpdateThreadRunningMutex.lock();
            bool isRunning = this->lightActionUpdateThreadRunning;
            this->lightActionUpdateThreadRunningMutex.unlock();
            
            if (isRunning) {
                this->lightActionUpdateThreadRunningMutex.lock();
                this->lightActionUpdateThreadRunning = false;
                this->lightActionUpdateThreadRunningMutex.unlock();

                this->lightActionUpdateThread.join();
                this->lightActionUpdateThread.~thread();
            }
        }

    }
}

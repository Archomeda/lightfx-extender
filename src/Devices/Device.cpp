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
            if (!this->isEnabled) {
                LOG(LogLevel::Info, L"Enabling");
                this->isEnabled = true;
            }
            return true;
        }

        LFXE_API bool Device::Disable() {
            if (this->isEnabled) {
                LOG(LogLevel::Info, L"Disabling");
                this->StopUpdateCurrentColor();
                this->isEnabled = false;
            }
            return true;
        }


        LFXE_API bool Device::Initialize() {
            if (!this->isInitialized) {
                LOG(LogLevel::Info, L"Initializing");
                this->Reset();
                this->isInitialized = true;
            }
            return true;
        }

        LFXE_API bool Device::Release() {
            if (this->isInitialized) {
                LOG(LogLevel::Info, L"Releasing");
                this->isInitialized = false;
            }
            return true;
        }

        LFXE_API bool Device::Update() {
            this->StopUpdateCurrentColor();
            this->CurrentLightAction = LightAction(this->QueuedLightAction);
            this->QueuedLightAction = LightAction();
            this->StartUpdateCurrentColor();
            return true;
        }

        LFXE_API bool Device::Reset() {
            this->CurrentLightAction = LightAction(LightActionType::Instant, this->numberOfLights, LightColor(0, 0, 0, 0), LightColor(0, 0, 0, 0), 200, 0, 0, 5);
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

                try {
                    this_thread::sleep_for(chrono::milliseconds(5));
                } catch (...) {
                    return;
                }
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
            if (this->lightActionUpdateThreadRunning) {
                this->lightActionUpdateThreadRunning = false;
            }
            this->lightActionUpdateThreadRunningMutex.unlock();

            if (this->lightActionUpdateThread.joinable()) {
                this->lightActionUpdateThread.join();
            }
        }

    }
}

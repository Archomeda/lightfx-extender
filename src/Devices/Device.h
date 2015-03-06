#pragma once

// Standard includes
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

// Project includes
#include "../Managers/DeviceManager.h"
#include "../Managers/ChildOfManager.h"
#include "../LightAction.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class DeviceManager;
    }

    namespace devices {

        enum LFXE_API DeviceType {
            DeviceUnknown,
            DeviceDisplay,
            DeviceKeyboard,
            DeviceOther
        };

        struct LFXE_API DeviceLightPosition {
            unsigned int x;
            unsigned int y;
            unsigned int z;
        };

        struct LFXE_API LightData {
            std::wstring Name = L"";
            DeviceLightPosition Position = {};
        };

        class LFXE_API Device : public managers::ChildOfManager < managers::DeviceManager > {

        public:
            Device() {}
            virtual ~Device();

            bool IsEnabled();
            bool IsInitialized();

            virtual bool Enable();
            virtual bool Disable();

            virtual bool Initialize();
            virtual bool Release();
            virtual bool Update();
            virtual bool Reset();

            LightAction GetActiveLightAction();
            LightAction GetQueuedLightAction();
            LightAction GetLastLightAction();
            void QueueLightAction(const LightAction& lightAction);

            virtual const std::wstring GetDeviceName() = 0;
            virtual const DeviceType GetDeviceType() = 0;

            const size_t GetNumberOfLights();
            LightData GetLightData(const size_t lightIndex);
            void SetLightData(const size_t lightIndex, const LightData& lightData);

        protected:
            void SetNumberOfLights(const size_t numberOfLights);

            LightAction ActiveLightAction = {};
            LightAction QueuedLightAction;
            std::queue<LightAction> LightActionQueue;
            std::mutex LightActionQueueMutex;

            void StartUpdateCurrentColorWorker();
            void StopUpdateCurrentColorWorker();
            void UpdateCurrentColorWorkerThread();
            virtual bool PushColorToDevice() = 0;

        private:
            bool isEnabled = false;
            bool isInitialized = false;
            size_t numberOfLights = 1;
            std::vector<LightData> lightData = {};

            std::thread lightActionUpdateThread;
            bool lightActionUpdateThreadRunning = false;
            std::mutex lightActionUpdateThreadMutex;
            std::condition_variable lightActionUpdateThreadConditionVariable;

        };

    }
}

#pragma warning(pop)

#pragma once

// Standard includes
#include <string>
#include <mutex>
#include <thread>

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

            LightAction GetCurrentLightAction();
            LightAction GetQueuedLightAction();
            void QueueLightAction(const LightAction& lightAction);
            
            virtual bool PushColorToDevice() = 0;

            virtual const std::wstring GetDeviceName() = 0;
            virtual const unsigned char GetDeviceType() = 0;

            virtual const size_t GetNumberOfLights() = 0;

        protected:
            LightAction CurrentLightAction = {};
            LightAction QueuedLightAction;

            virtual void UpdateCurrentColorLoop();
            virtual void StartUpdateCurrentColor();
            virtual void StopUpdateCurrentColor();

        private:
            bool isEnabled = false;
            bool isInitialized = false;

            std::thread lightActionUpdateThread;
            bool lightActionUpdateThreadRunning = false;
            std::mutex lightActionUpdateThreadRunningMutex;

        };

    }
}

#pragma warning(pop)

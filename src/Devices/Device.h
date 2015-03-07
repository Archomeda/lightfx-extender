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
#include "../LightColorsTimeline.h"

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
            virtual bool Update(bool flushQueue = true);
            virtual bool Reset();

            LightColorsTimeline GetActiveTimeline();
            LightColorsTimeline GetQueuedTimeline();
            LightColorsTimeline GetRecentTimeline();
            void QueueTimeline(const LightColorsTimeline& timeline);

            virtual const std::wstring GetDeviceName() = 0;
            virtual const DeviceType GetDeviceType() = 0;

            const size_t GetNumberOfLights();
            LightColor GetLightColor(const size_t lightIndex);
            LightData GetLightData(const size_t lightIndex);
            void SetLightData(const size_t lightIndex, const LightData& lightData);

        protected:
            void SetNumberOfLights(const size_t numberOfLights);

            LightColorsTimeline ActiveTimeline;
            LightColorsTimeline QueuedTimeline;
            std::queue<LightColorsTimeline> TimelineQueue;
            std::mutex TimelineQueueMutex;
            bool TimelineQueueFlush = false;

            void StartLightColorUpdateWorker();
            void StopLightColorUpdateWorker();
            void LightColorUpdateWorkerThread();
            virtual bool PushColorToDevice(const std::vector<LightColor>& colors) = 0;

        private:
            bool isEnabled = false;
            bool isInitialized = false;
            size_t numberOfLights = 0;
            std::vector<LightColor> lightColor = {};
            std::vector<LightData> lightData = {};

            std::thread lightColorUpdateThread;
            bool lightColorUpdateThreadRunning = false;
            std::mutex lightColorUpdateThreadMutex;
            std::condition_variable lightColorUpdateThreadConditionVariable;

        };

    }
}

#pragma warning(pop)

#pragma once

// Standard includes
#include <atomic>
#include <thread>

// Project includes
#include "ManagerWithChildren.h"
#include "../Devices/Device.h"
#include "../Devices/Proxies/LightFX2Proxy.h"
#include "../Utils/NotifyEvent.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace devices {
        class Device;
    }

    namespace managers {

        class LFXE_API DeviceManager : public ManagerWithChildren < devices::Device, DeviceManager > {

        public:
            DeviceManager() {};

            size_t InitializeDevices();
            size_t UninitializeDevices();

            void StartUpdateDevicesWorker();
            void StopUpdateDevicesWorker();
            void UpdateDeviceLights(const bool flushQueue = true);

        protected:
            void UpdateDevicesWorker();

        private:
            std::unique_ptr<devices::proxies::LightFX2Proxy> lightFXLibrary;
            
            bool flushQueue = false;

            bool updateDevicesWorkerActive = false;
            std::atomic<bool> stopUpdateDevicesWorker = false;
            std::thread updateDevicesWorkerThread;
            utils::NotifyEvent updateDevicesNotifyEvent;

            int updateDevicesInterval = 20;

        };

    }
}

#pragma warning(pop)

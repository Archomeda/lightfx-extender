#pragma once

// Standard includes
#include <memory>

// Project includes
#include "ManagerWithChildren.h"
#include "../Devices/Device.h"
#include "../Utils/Timer.h"

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

            void StartUpdateTimer();
            void StopUpdateTimer();

        protected:
            void DeviceUpdateTask();

        private:
            std::unique_ptr<utils::Timer> DeviceUpdateTimer;

        };

    }
}

#pragma warning(pop)

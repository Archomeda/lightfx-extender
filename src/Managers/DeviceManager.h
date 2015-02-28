#pragma once

// Project includes
#include "ManagerWithChildren.h"
#include "../Devices/Device.h"

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

        };

    }
}

#pragma warning(pop)

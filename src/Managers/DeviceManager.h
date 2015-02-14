#pragma once

// Project includes
#include "Manager.h"
#include "../Devices/Device.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace devices {
        class Device;
    }

    namespace managers {

        class LFXE_API DeviceManager : public Manager < devices::Device, DeviceManager > {

        public:
            DeviceManager() {};

        };

        template class LFXE_API Manager < devices::Device, DeviceManager > ;

    }
}

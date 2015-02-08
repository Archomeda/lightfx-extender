#pragma once

// Project includes
#include "Device.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace devices {

        class Device;

        class LFXE_API LightManager {

        public:
            LightManager(const Device* device);
            ~LightManager();

        private:


        };

    }
}

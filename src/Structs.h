#pragma once

// Standard includes
#include <vector>
#include <string>

// 3rd party includes
#include "LFXDecl.h"


namespace lightfx {

    struct DeviceLightData {
        std::string Description = "";
        LFX_POSITION Position = {};
    };

    struct DeviceData {
        std::string Description = "";
        unsigned int Type = LFX_DEVTYPE_UNKNOWN;
        std::vector<DeviceLightData> Lights = {};
    };

}

#pragma once

// Standard includes
#include <vector>
#include <string>

// 3rd party includes
#include "LFXDecl.h"


namespace lightfx {
    namespace devices {

        struct DeviceLight {
            std::wstring Name = L"";
            LFX_POSITION Position = {};
        };

    }
}

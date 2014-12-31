#pragma once

#include <stdint.h>
#include "LFXDecl.h"

namespace lightfx {
    namespace api {
        
        struct LFXLight {
            LFX_POSITION position;
            uint32_t actionType;
            LFX_COLOR primaryColor;
            LFX_COLOR secondaryColor;
        };

        struct LinkedMem {
            uint32_t version; // Current version: 0
            uint32_t tick;

            uint32_t isInitialized;
            int32_t timing;

            LFXLight light;
        };

        extern LinkedMem* linkedMemHandle;

        void init();

        void dispose();

    }
}

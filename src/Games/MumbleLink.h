#pragma once

// Standard includes
#include <memory>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>


namespace lightfx {
    namespace games {

        struct MumbleLinkedMem {
            uint32_t uiVersion;
            uint32_t uiTick;
            float	fAvatarPosition[3];
            float	fAvatarFront[3];
            float	fAvatarTop[3];
            wchar_t	name[256];
            float	fCameraPosition[3];
            float	fCameraFront[3];
            float	fCameraTop[3];
            wchar_t	identity[256];
            uint32_t context_len;
            unsigned char context[256];
            wchar_t description[2048];
        };

    }
}
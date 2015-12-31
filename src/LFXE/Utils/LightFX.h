#pragma once

// Standard includes
#include <string>

// 3rd party includes
#include "LFX2.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace utils {

        LFXE_API std::wstring GetFriendlyLfxResult(LFX_RESULT result);

    }
}

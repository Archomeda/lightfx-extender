#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightFX.h"


using namespace std;

namespace lightfx {
    namespace utils {

        LFXE_API wstring GetFriendlyLfxResult(LFX_RESULT result) {
            switch (result)
            {
            case LFX_SUCCESS:
                return L"LFX_SUCCESS";
            case LFX_FAILURE:
                return L"LFX_FAILURE";
            case LFX_ERROR_NOINIT:
                return L"LFX_ERROR_NOINIT";
            case LFX_ERROR_NODEVS:
                return L"LFX_ERROR_NODEVS";
            case LFX_ERROR_NOLIGHTS:
                return L"LFX_ERROR_NOLIGHTS";
            case LFX_ERROR_BUFFSIZE:
                return L"LFX_ERROR_BUFFSIZE";
            default:
                return to_wstring(result);
            }
        }

    }
}

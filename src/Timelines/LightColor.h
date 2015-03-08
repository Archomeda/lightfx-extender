#pragma once

// Standard includes
#include <string>
#include <vector>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace timelines {

        struct LFXE_API LightColor {
            LightColor() {}
            LightColor(int red, int green, int blue, int brightness) : red(red), green(green), blue(blue), brightness(brightness) {}

            int red;
            int green;
            int blue;
            int brightness;

            friend inline bool operator==(const LightColor& a, const LightColor& b) {
                return a.red == b.red && a.green == b.green && a.blue == b.blue && a.brightness == b.brightness;
            }
            friend inline bool operator!=(const LightColor& a, const LightColor& b) { return !operator==(a, b); }
        };

    }
}

#pragma once

namespace lightfx {

    enum LightLocationMask {
        FrontLowerLeft = 1 << 1,
        FrontLowerCenter = 1 << 2,
        FrontLowerRight = 1 << 3,
        FrontMiddleLeft = 1 << 4,
        FrontMiddleCenter = 1 << 5,
        FrontMiddleRight = 1 << 6,
        FrontUpperLeft = 1 << 7,
        FrontUpperCenter = 1 << 8,
        FrontUpperRight = 1 << 9,
        MiddleLowerLeft = 1 << 10,
        MiddleLowerCenter = 1 << 11,
        MiddleLowerRight = 1 << 12,
        MiddleMiddleLeft = 1 << 13,
        MiddleMiddleCenter = 1 << 14,
        MiddleMiddleRight = 1 << 15,
        MiddleUpperLeft = 1 << 16,
        MiddleUpperCenter = 1 << 17,
        MiddleUpperRight = 1 << 18,
        BackLowerLeft = 1 << 19,
        BackLowerCenter = 1 << 20,
        BackLowerRight = 1 << 21,
        BackMiddleLeft = 1 << 22,
        BackMiddleCenter = 1 << 23,
        BackMiddleRight = 1 << 24,
        BackUpperLeft = 1 << 25,
        BackUpperCenter = 1 << 26,
        BackUpperRight = 1 << 27,
    };

    inline LightLocationMask operator |(LightLocationMask a, LightLocationMask b) {
        return static_cast<LightLocationMask>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline LightLocationMask& operator |=(LightLocationMask& a, LightLocationMask b) {
        return a = a | b;
    }
}

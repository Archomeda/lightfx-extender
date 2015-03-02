#pragma once

// API exports
#include "Common/ApiExports.h"


namespace lightfx {

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

    enum LFXE_API LightActionType {
        Instant,
        Morph,
        Pulse,
    };

    class LFXE_API LightAction {

    public:
        LightAction() {}
        LightAction(const LightColor& color)
            : actionType(LightActionType::Instant), startColor(color) {}
        LightAction(const LightActionType& actionType, const LightColor& color, unsigned int actionTime)
            : actionType(actionType), startColor(color), actionTime(actionTime) {}
        LightAction(const LightActionType& actionType, const LightColor& color, unsigned int actionTime, unsigned int actionRepeatAmount)
            : actionType(actionType), startColor(color), actionTime(actionTime), actionRepeatAmount(actionRepeatAmount) {}
        LightAction(const LightActionType& actionType, const LightColor& startColor, const LightColor& endColor, unsigned int actionTime, unsigned int startColorHoldTime, unsigned int endColorHoldTime, unsigned int actionRepeatAmount)
            : actionType(actionType), startColor(startColor), endColor(endColor), actionTime(actionTime), startColorHoldTime(startColorHoldTime), endColorHoldTime(endColorHoldTime), actionRepeatAmount(actionRepeatAmount) {}

        LightColor GetStartColor();
        LightColor GetEndColor();
        LightActionType GetActionType();
        unsigned int GetActionTime();
        unsigned int GetStartColorHoldTime();
        unsigned int GetEndColorHoldTime();
        unsigned int GetActionRepeatAmount();

        void SetStartColor(const LightColor& startColor);
        void SetEndColor(const LightColor& endColor);
        void SetActionType(const LightActionType& actionType);
        void SetActionTime(const unsigned int actionTime);
        void SetStartColorHoldTime(const unsigned int startColorHoldTime);
        void SetEndColorHoldTime(const unsigned int endColorHoldTime);
        void SetActionRepeatAmount(const unsigned int actionRepeatAmount);

    private:
        LightColor startColor;
        LightColor endColor;
        LightActionType actionType;

        unsigned int actionTime = 200;
        unsigned int startColorHoldTime = 0;
        unsigned int endColorHoldTime = 0;
        unsigned int actionRepeatAmount = 5;


    };


}

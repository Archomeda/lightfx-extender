#pragma once

// Standard includes
#include <vector>

// API exports
#include "Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

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

        LightAction(const size_t numLights, const LightColor& color)
            : actionType(LightActionType::Instant), currentColor(numLights), startColor(numLights, color), endColor(numLights) {}

        LightAction(const LightActionType& actionType, const size_t numLights, const LightColor& color, unsigned int actionTime)
            : actionType(actionType), currentColor(numLights), startColor(numLights, color), endColor(numLights), actionTime(actionTime) {}

        LightAction(const LightActionType& actionType, const size_t numLights, const LightColor& color, unsigned int actionTime, unsigned int actionRepeatAmount)
            : actionType(actionType), currentColor(numLights), startColor(numLights, color), endColor(numLights), actionTime(actionTime), actionRepeatAmount(actionRepeatAmount) {}

        LightAction(const LightActionType& actionType, const size_t numLights, const LightColor& startColor, const LightColor& endColor, unsigned int actionTime, unsigned int startColorHoldTime, unsigned int endColorHoldTime, unsigned int actionRepeatAmount)
            : actionType(actionType), currentColor(numLights), startColor(numLights, startColor), endColor(numLights, endColor), actionTime(actionTime), startColorHoldTime(startColorHoldTime), endColorHoldTime(endColorHoldTime), actionRepeatAmount(actionRepeatAmount) {}

        LightColor GetStartColor(size_t lightIndex);
        std::vector<LightColor> GetStartColor();
        LightColor GetEndColor(size_t lightIndex);
        std::vector<LightColor> GetEndColor();
        LightActionType GetActionType();
        unsigned int GetActionTime();
        unsigned int GetStartColorHoldTime();
        unsigned int GetEndColorHoldTime();
        unsigned int GetActionRepeatAmount();
        
        void SetStartColor(const LightColor& startColor);
        void SetStartColor(const size_t lightIndex, const LightColor& startColor);
        void SetStartColor(const std::vector<LightColor>& startColor);
        void SetEndColor(const LightColor& endColor);
        void SetEndColor(const size_t lightIndex, const LightColor& endColor);
        void SetEndColor(const std::vector<LightColor>& endColor);
        void SetActionType(const LightActionType& actionType);
        void SetActionTime(const unsigned int actionTime);
        void SetStartColorHoldTime(const unsigned int startColorHoldTime);
        void SetEndColorHoldTime(const unsigned int endColorHoldTime);
        void SetActionRepeatAmount(const unsigned int actionRepeatAmount);

        LightColor GetCurrentColor(size_t lightIndex);
        std::vector<LightColor> GetCurrentColor();
        bool CanUpdateCurrentColor();
        bool UpdateCurrentColor();

    private:
        std::vector<LightColor> currentColor;
        unsigned long animatedColorStartTime = 0;
        bool canUpdateCurrentColor = true;
        bool UpdateCurrentColorMorph(const unsigned long timePassed);
        bool UpdateCurrentColorPulse(const unsigned long timePassed);

        std::vector<LightColor> startColor;
        std::vector<LightColor> endColor;
        LightActionType actionType;

        std::vector<LightColor> prevColor;

        unsigned int actionTime = 200;
        unsigned int startColorHoldTime = 0;
        unsigned int endColorHoldTime = 0;
        unsigned int actionRepeatAmount = 5;


    };


}

#pragma warning(pop)

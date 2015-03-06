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

        LightAction(const size_t numLights)
            : actionType(LightActionType::Instant), currentColor(numLights), startColor(numLights), endColor(numLights), resetColor(numLights) {}

        static LightAction NewInstant(const size_t numLights, const LightColor& color) {
            return LightAction(numLights, color);
        }

        static LightAction NewMorph(const size_t numLights, const LightColor& startColor, const LightColor& endColor, unsigned int morphTime) {
            return LightAction(numLights, startColor, endColor, morphTime);
        }

        static LightAction NewPulse(const size_t numLights, const LightColor& startColor, const LightColor& endColor, unsigned int pulseTime, unsigned int pulseAmount) {
            return NewPulse(numLights, startColor, endColor, pulseTime, 0, 0, pulseAmount);
        }

        static LightAction NewPulse(const size_t numLights, const LightColor& startColor, const LightColor& endColor, unsigned int pulseTime, unsigned int startColorHoldTime, unsigned int endColorHoldTime, unsigned int pulseAmount) {
            return NewPulse(numLights, startColor, endColor, startColor, pulseTime, startColorHoldTime, endColorHoldTime, pulseAmount);
        }

        static LightAction NewPulse(const size_t numLights, const LightColor& startColor, const LightColor& endColor, const LightColor& resetColor, unsigned int pulseTime, unsigned int startColorHoldTime, unsigned int endColorHoldTime, unsigned int pulseAmount) {
            return LightAction(numLights, startColor, endColor, resetColor, pulseTime, startColorHoldTime, endColorHoldTime, pulseAmount);
        }

        LightColor GetStartColor(size_t lightIndex);
        std::vector<LightColor> GetStartColor();
        LightColor GetEndColor(size_t lightIndex);
        std::vector<LightColor> GetEndColor();
        LightColor GetResetColor(size_t lightIndex);
        std::vector<LightColor> GetResetColor();
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
        void SetResetColor(const LightColor& resetColor);
        void SetResetColor(const size_t lightIndex, const LightColor& resetColor);
        void SetResetColor(const std::vector<LightColor>& resetColor);
        void SetActionType(const LightActionType& actionType);
        void SetActionTime(const unsigned int actionTime);
        void SetStartColorHoldTime(const unsigned int startColorHoldTime);
        void SetEndColorHoldTime(const unsigned int endColorHoldTime);
        void SetActionRepeatAmount(const unsigned int actionRepeatAmount);

        LightColor GetCurrentColor(size_t lightIndex);
        std::vector<LightColor> GetCurrentColor();
        bool CanUpdateCurrentColor();
        bool UpdateCurrentColor();

    protected:
        LightAction(const size_t numLights, const LightColor& color)
            : actionType(LightActionType::Instant), currentColor(numLights), startColor(numLights, color), endColor(numLights, color), resetColor(numLights, color) {}

        LightAction(const size_t numLights, const LightColor& startColor, const LightColor& endColor, unsigned int actionTime)
            : actionType(LightActionType::Morph), currentColor(numLights), startColor(numLights, startColor), endColor(numLights, endColor), resetColor(numLights, endColor), actionTime(actionTime) {}

        LightAction(const size_t numLights, const LightColor& startColor, const LightColor& endColor, const LightColor& resetColor, unsigned int actionTime, unsigned int startColorHoldTime, unsigned int endColorHoldTime, unsigned int actionRepeatAmount)
            : actionType(LightActionType::Pulse), currentColor(numLights), startColor(numLights, startColor), endColor(numLights, endColor), resetColor(numLights, resetColor), actionTime(actionTime), startColorHoldTime(startColorHoldTime), endColorHoldTime(endColorHoldTime), actionRepeatAmount(actionRepeatAmount) {}

    private:
        std::vector<LightColor> currentColor = {};
        unsigned long long animatedColorStartTime = 0;
        bool canUpdateCurrentColor = true;
        bool UpdateCurrentColorMorph(const unsigned long long timePassed);
        bool UpdateCurrentColorPulse(const unsigned long long timePassed);
        bool UpdateCurrentColorInstant();

        std::vector<LightColor> startColor = {};
        std::vector<LightColor> endColor = {};
        std::vector<LightColor> resetColor = {};
        LightActionType actionType = LightActionType::Instant;

        unsigned int actionTime = 200;
        unsigned int startColorHoldTime = 0;
        unsigned int endColorHoldTime = 0;
        unsigned int actionRepeatAmount = 5;

    };

}

#pragma warning(pop)

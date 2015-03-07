#pragma once

// Standard includes
#include <vector>

// API exports
#include "Common/ApiExports.h"

// Project includes
#include "LightColorTimelineAction.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {

    class LFXE_API LightColorTimeline {

    public:
        LightColorTimeline() {}
        virtual ~LightColorTimeline() {}

        void AddAction(const LightColorTimelineAction& action);

        unsigned long GetTotalDuration();
        LightColor GetColorAtTime(const unsigned long time);


        static LightColorTimeline NewInstant(const LightColor& color);
        static LightColorTimeline NewMorph(const LightColor& startColor, const LightColor& endColor, const unsigned long morphTime, const unsigned long startDelay = 0);
        static LightColorTimeline NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startDelay = 0);
        static LightColorTimeline NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const unsigned long startDelay = 0);
        static LightColorTimeline NewPulse(const LightColor& startColor, const LightColor& endColor, const LightColor& resetColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const unsigned long startDelay = 0);

    private:
        std::vector<LightColorTimelineAction> actions = {};
        unsigned long totalDuration = 0;

    };

}

#pragma warning(pop)

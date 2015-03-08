#pragma once

// Standard includes
#include <string>
#include <vector>

// Project includes
#include "LightColor.h"
#include "LightTimelineItem.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace timelines {

        class LFXE_API LightTimeline {

        public:
            LightTimeline() {}
            virtual ~LightTimeline() {}

            void AddItem(const LightTimelineItem& item);

            unsigned long GetTotalDuration();
            LightColor GetColorAtTime(const unsigned long time);

            std::wstring ToString();

            static LightTimeline NewInstant(const LightColor& color);
            static LightTimeline NewMorph(const LightColor& startColor, const LightColor& endColor, const unsigned long morphTime, const long startDelay = 0);
            static LightTimeline NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const long startDelay = 0);
            static LightTimeline NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const long startDelay = 0);
            static LightTimeline NewPulse(const LightColor& startColor, const LightColor& endColor, const LightColor& resetColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const long startDelay = 0);

        private:
            std::vector<LightTimelineItem> items = {};
            unsigned long totalDuration = 0;

        };

    }
}

#pragma warning(pop)

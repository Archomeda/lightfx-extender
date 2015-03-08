#pragma once

// Standard includes
#include <string>
#include <vector>

// Project includes
#include "LightColor.h"
#include "LightTimeline.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace timelines {

        class LFXE_API Timeline {

        public:
            Timeline() {}
            Timeline(const size_t numLights, const LightTimeline& timeline) : timeline(numLights, timeline) {}
            Timeline(const std::vector<LightTimeline>& timeline) : timeline(timeline) {}

            LightTimeline GetTimeline(const size_t lightIndex);
            std::vector<LightTimeline> GetTimeline();

            void SetTimeline(const size_t lightIndex, const LightTimeline& timeline);
            void SetTimeline(const std::vector<LightTimeline> timeline);

            unsigned long GetTotalDuration();
            unsigned long GetTotalDuration(const size_t lightIndex);
            std::vector<LightColor> GetColorAtTime(const unsigned long time);
            LightColor GetColorAtTime(const size_t lightIndex, const unsigned long time);

            std::wstring ToString();

            static Timeline NewWalk(const size_t numLights, const LightColor& baseColor, const LightColor& pulseColor, const std::vector<LightColor>& resetColor, const unsigned long walkTime, const unsigned long walkAmount, const long startDelay = 0);

        private:
            std::vector<LightTimeline> timeline = {};

        };
    
    }
}

#pragma warning(pop)

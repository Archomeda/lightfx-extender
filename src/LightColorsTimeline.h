#pragma once

// Standard includes
#include <string>
#include <vector>

// Project includes
#include "LightColorTimeline.h"

// API exports
#include "Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {

    class LFXE_API LightColorsTimeline {

    public:
        LightColorsTimeline() {}
        LightColorsTimeline(const size_t numLights, const LightColorTimeline& timeline) : timeline(numLights, timeline) {}
        LightColorsTimeline(const std::vector<LightColorTimeline>& timeline) : timeline(timeline) {}

        LightColorTimeline GetTimeline(const size_t lightIndex);
        std::vector<LightColorTimeline> GetTimeline();

        void SetTimeline(const size_t lightIndex, const LightColorTimeline& timeline);
        void SetTimeline(const std::vector<LightColorTimeline> timeline);

        unsigned long GetTotalDuration();
        unsigned long GetTotalDuration(const size_t lightIndex);
        std::vector<LightColor> GetColorAtTime(const unsigned long time);
        LightColor GetColorAtTime(const size_t lightIndex, const unsigned long time);

        std::wstring ToString();

    private:
        std::vector<LightColorTimeline> timeline = {};

    };

}

#pragma warning(pop)

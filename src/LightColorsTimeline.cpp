#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightColorsTimeline.h"

// Standard includes
#include <algorithm>


using namespace std;

namespace lightfx {

    LFXE_API LightColorTimeline LightColorsTimeline::GetTimeline(const size_t lightIndex) {
        return this->timeline[lightIndex];
    }

    LFXE_API vector<LightColorTimeline> LightColorsTimeline::GetTimeline() {
        return this->timeline;
    }


    LFXE_API void LightColorsTimeline::SetTimeline(const size_t lightIndex, const LightColorTimeline& timeline) {
        if (this->timeline.size() <= lightIndex) {
            this->timeline.resize(lightIndex + 1);
        }
        this->timeline[lightIndex] = timeline;
    }

    LFXE_API void LightColorsTimeline::SetTimeline(const vector<LightColorTimeline> timeline) {
        this->timeline = timeline;
    }


    LFXE_API unsigned long LightColorsTimeline::GetTotalDuration() {
        unsigned long duration = 0;
        for (size_t i = 0; i < this->timeline.size(); ++i) {
            duration = max(duration, this->timeline[i].GetTotalDuration());
        }
        return duration;
    }

    LFXE_API unsigned long LightColorsTimeline::GetTotalDuration(const size_t lightIndex) {
        return this->timeline[lightIndex].GetTotalDuration();
    }
    
    LFXE_API vector<LightColor> LightColorsTimeline::GetColorAtTime(const unsigned long time) {
        vector<LightColor> colors = {};
        for (size_t i = 0; i < this->timeline.size(); ++i) {
            colors.push_back(this->timeline[i].GetColorAtTime(time));
        }
        return colors;
    }

    LFXE_API LightColor LightColorsTimeline::GetColorAtTime(const size_t lightIndex, const unsigned long time) {
        return this->timeline[lightIndex].GetColorAtTime(time);
    }

    LFXE_API wstring LightColorsTimeline::ToString() {
        wstring result;
        for (size_t i = 0; i < this->timeline.size(); ++i) {
            if (i != 0) {
                result += L",";
            }
            result += to_wstring(i) + L":(" + this->timeline[i].ToString() + L")";
        }
        return result;
    }

}

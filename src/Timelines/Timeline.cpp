#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Timeline.h"

// Standard includes
#include <algorithm>


using namespace std;

namespace lightfx {
    namespace timelines {

        LFXE_API bool Timeline::IsValid() const {
            return this->timeline.size() > 0;
        }

        LFXE_API LightTimeline Timeline::GetTimeline(const size_t lightIndex) const {
            return this->timeline[lightIndex];
        }

        LFXE_API vector<LightTimeline> Timeline::GetTimeline() const {
            return this->timeline;
        }


        LFXE_API void Timeline::SetTimeline(const size_t lightIndex, const LightTimeline& timeline) {
            if (this->timeline.size() <= lightIndex) {
                this->timeline.resize(lightIndex + 1);
            }
            this->timeline[lightIndex] = timeline;
        }

        LFXE_API void Timeline::SetTimeline(const vector<LightTimeline> timeline) {
            this->timeline = timeline;
        }


        LFXE_API unsigned long Timeline::GetTotalDuration() const {
            unsigned long duration = 0;
            for (size_t i = 0; i < this->timeline.size(); ++i) {
                duration = max(duration, this->timeline[i].GetTotalDuration());
            }
            return duration;
        }

        LFXE_API unsigned long Timeline::GetTotalDuration(const size_t lightIndex) const {
            return this->timeline[lightIndex].GetTotalDuration();
        }

        LFXE_API vector<LightColor> Timeline::GetColorAtTime(const unsigned long time) const {
            vector<LightColor> colors = {};
            for (size_t i = 0; i < this->timeline.size(); ++i) {
                colors.push_back(this->timeline[i].GetColorAtTime(time));
            }
            return colors;
        }

        LFXE_API LightColor Timeline::GetColorAtTime(const size_t lightIndex, const unsigned long time) const {
            return this->timeline[lightIndex].GetColorAtTime(time);
        }

        LFXE_API wstring Timeline::ToString() const {
            wstring result;
            for (size_t i = 0; i < this->timeline.size(); ++i) {
                if (i != 0) {
                    result += L",";
                }
                result += to_wstring(i) + L":(" + this->timeline[i].ToString() + L")";
            }
            return result;
        }

        LFXE_API Timeline Timeline::NewWalk(const size_t numLights, const LightColor& baseColor, const LightColor& pulseColor, const vector<LightColor>& resetColor, const unsigned long walkTime, const unsigned long walkAmount, const long startDelay) {
            if (numLights <= 4) {
                vector<LightTimeline> lightTimeline(numLights);
                for (size_t i = 0; i < numLights; ++i) {
                    lightTimeline[i] = LightTimeline::NewPulse(baseColor, pulseColor, resetColor[i], walkTime, walkAmount, 0, 0, startDelay);
                }
                return Timeline(lightTimeline);
            }
            
            unsigned int numLights2;
            if (numLights > numeric_limits<unsigned int>::max()) {
                numLights2 = numeric_limits<unsigned int>::max();
            } else {
                numLights2 = static_cast<unsigned int>(numLights);
            }

            unsigned int divider = numLights2 / 4;
            unsigned long pulseTime = divider * walkTime / numLights2;
            unsigned long pulseColorHoldTime = pulseTime;
            unsigned long stepTime = pulseTime / divider;
            long transitionTime = long(pulseTime / 2) - pulseColorHoldTime;
            if (transitionTime < 0) {
                transitionTime = 0;
            }

            Timeline timeline;
            unsigned long baseColorHoldTime = walkTime - pulseTime - (pulseColorHoldTime * 2);
            long walkStartDelay = startDelay - baseColorHoldTime / 2;
            long walkEndDelay = walkTime + 1; // Plus one to ensure we have no overlap in the last timeline items
            for (size_t i = 0; i < numLights; ++i) {
                LightTimeline lightTimeline = LightTimeline::NewPulse(baseColor, pulseColor, baseColor, pulseTime, walkAmount, baseColorHoldTime, pulseColorHoldTime, walkStartDelay);
                lightTimeline.AddItem(LightTimelineItem(resetColor[i], 0, walkEndDelay));
                timeline.SetTimeline(i, lightTimeline);

                walkStartDelay += stepTime;
                walkEndDelay -= stepTime;
            }
            return timeline;
        }
    }
}

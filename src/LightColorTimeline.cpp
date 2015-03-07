#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightColorTimeline.h"


using namespace std;

namespace lightfx {

    LFXE_API void LightColorTimeline::AddAction(const LightColorTimelineAction& action) {
        this->actions.push_back(action);
        this->totalDuration += this->actions.back().GetStartDelay() + this->actions.back().GetDuration();
    }

    LFXE_API unsigned long LightColorTimeline::GetTotalDuration() {
        return this->totalDuration;
    }

    LFXE_API LightColor LightColorTimeline::GetColorAtTime(const unsigned long time) {
        if (this->actions.size() == 0) {
            return LightColor();
        }

        unsigned long timeStart = 0;
        long timeLeft = time - this->actions[0].GetStartDelay();
        size_t currentIndex = 0;
        while (timeLeft > 0 && currentIndex < this->actions.size()) {
            timeLeft -= this->actions[currentIndex].GetDuration();
            if (timeLeft > 0) {
                timeStart += this->actions[currentIndex].GetDuration();
                if (currentIndex > 0) {
                    timeStart += this->actions[currentIndex].GetStartDelay();
                }

                ++currentIndex;
                if (currentIndex < this->actions.size()) {
                    timeLeft -= this->actions[currentIndex].GetStartDelay();
                }
            }
        }

        if (currentIndex < this->actions.size()) {
            long timeRelative = time - timeStart - this->actions[currentIndex].GetStartDelay();
            if (timeRelative < 0) {
                return this->actions[currentIndex - 1].GetColor();
            } else if (timeRelative == 0) {
                if (this->actions[currentIndex].GetDuration() == 0) {
                    return this->actions[currentIndex].GetColor();
                } else {
                    return this->actions[currentIndex - 1].GetColor();
                }
            } else {
                LightColor colorStart = this->actions[currentIndex - 1].GetColor();
                LightColor colorEnd = this->actions[currentIndex].GetColor();
                double progress = (double)timeRelative / this->actions[currentIndex].GetDuration();

                return LightColor(
                    int(colorStart.red + progress * (colorEnd.red - colorStart.red)),
                    int(colorStart.green + progress * (colorEnd.green - colorStart.green)),
                    int(colorStart.blue + progress * (colorEnd.blue - colorStart.blue)),
                    int(colorStart.brightness + progress * (colorEnd.brightness - colorStart.brightness))
                    );
            }
        } else {
            return this->actions[currentIndex - 1].GetColor();
        }
    }

    LFXE_API wstring LightColorTimeline::ToString() {
        wstring result;
        for (size_t i = 0; i < this->actions.size(); ++i) {
            if (i != 0) {
                result += L"->";
            }
            result += L"(" + this->actions[i].ToString() + L")";
        }
        return result;
    }


    LFXE_API LightColorTimeline LightColorTimeline::NewInstant(const LightColor& color) {
        LightColorTimeline timeline;
        timeline.AddAction(LightColorTimelineAction(color, 0, 0));
        return timeline;
    }

    LFXE_API LightColorTimeline LightColorTimeline::NewMorph(const LightColor& startColor, const LightColor& endColor, const unsigned long morphTime, const unsigned long startDelay) {
        LightColorTimeline timeline;
        timeline.AddAction(LightColorTimelineAction(startColor, 0, 0));
        timeline.AddAction(LightColorTimelineAction(endColor, morphTime, startDelay));
        return timeline;
    }

    LFXE_API LightColorTimeline LightColorTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startDelay) {
        return LightColorTimeline::NewPulse(startColor, endColor, pulseTime, pulseAmount, 0, 0, startDelay);
    }

    LFXE_API LightColorTimeline LightColorTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const unsigned long startDelay) {
        return LightColorTimeline::NewPulse(startColor, endColor, startColor, pulseTime, pulseAmount, 0, 0, startDelay);
    }

    LFXE_API LightColorTimeline LightColorTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const LightColor& resetColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const unsigned long startDelay) {
        LightColorTimelineAction actionStart(startColor, 0, 0);

        LightColorTimelineAction actionStartToEndColorFirst(endColor, pulseTime, startDelay + startColorHoldTime / 2);
        LightColorTimelineAction actionStartToEndColorOther(endColor, pulseTime, startColorHoldTime);
        LightColorTimelineAction actionEndToStartColor(startColor, pulseTime, endColorHoldTime);

        LightColorTimelineAction actionReset(resetColor, 0, startColorHoldTime / 2);

        LightColorTimeline timeline;
        timeline.AddAction(actionStart);
        for (unsigned long i = 0; i < pulseAmount; ++i) {
            if (i == 0) {
                timeline.AddAction(actionStartToEndColorFirst);
            } else {
                timeline.AddAction(actionStartToEndColorOther);
            }
            timeline.AddAction(actionEndToStartColor);
        }
        timeline.AddAction(actionReset);
        return timeline;
    }

}

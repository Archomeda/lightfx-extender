#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightTimeline.h"


using namespace std;

namespace lightfx {
    namespace timelines {

        LFXE_API void LightTimeline::AddItem(const LightTimelineItem& item) {
            this->items.push_back(item);
            this->totalDuration += this->items.back().GetStartDelay() + this->items.back().GetDuration();
        }

        LFXE_API unsigned long LightTimeline::GetTotalDuration() {
            return this->totalDuration;
        }

        LFXE_API LightColor LightTimeline::GetColorAtTime(const unsigned long time) {
            if (this->items.size() == 0) {
                return LightColor();
            }

            unsigned long timeStart = 0;
            long timeLeft = time - this->items[0].GetStartDelay();
            size_t currentIndex = 0;
            while (timeLeft > 0 && currentIndex < this->items.size()) {
                timeLeft -= this->items[currentIndex].GetDuration();
                if (timeLeft > 0) {
                    timeStart += this->items[currentIndex].GetDuration();
                    if (currentIndex > 0) {
                        timeStart += this->items[currentIndex].GetStartDelay();
                    }

                    ++currentIndex;
                    if (currentIndex < this->items.size()) {
                        timeLeft -= this->items[currentIndex].GetStartDelay();
                    }
                }
            }

            if (currentIndex < this->items.size()) {
                long timeRelative = time - timeStart - this->items[currentIndex].GetStartDelay();
                if (timeRelative < 0) {
                    return this->items[currentIndex - 1].GetColor();
                } else if (timeRelative == 0) {
                    if (this->items[currentIndex].GetDuration() == 0) {
                        return this->items[currentIndex].GetColor();
                    } else {
                        return this->items[currentIndex - 1].GetColor();
                    }
                } else {
                    LightColor colorStart = this->items[currentIndex - 1].GetColor();
                    LightColor colorEnd = this->items[currentIndex].GetColor();
                    double progress = (double)timeRelative / this->items[currentIndex].GetDuration();

                    return LightColor(
                        int(colorStart.red + progress * (colorEnd.red - colorStart.red)),
                        int(colorStart.green + progress * (colorEnd.green - colorStart.green)),
                        int(colorStart.blue + progress * (colorEnd.blue - colorStart.blue)),
                        int(colorStart.brightness + progress * (colorEnd.brightness - colorStart.brightness))
                        );
                }
            } else {
                return this->items[currentIndex - 1].GetColor();
            }
        }

        LFXE_API wstring LightTimeline::ToString() {
            wstring result;
            for (size_t i = 0; i < this->items.size(); ++i) {
                if (i != 0) {
                    result += L"->";
                }
                result += L"(" + this->items[i].ToString() + L")";
            }
            return result;
        }


        LFXE_API LightTimeline LightTimeline::NewInstant(const LightColor& color) {
            LightTimeline timeline;
            timeline.AddItem(LightTimelineItem(color, 0, 0));
            return timeline;
        }

        LFXE_API LightTimeline LightTimeline::NewMorph(const LightColor& startColor, const LightColor& endColor, const unsigned long morphTime, const unsigned long startDelay) {
            LightTimeline timeline;
            timeline.AddItem(LightTimelineItem(startColor, 0, 0));
            timeline.AddItem(LightTimelineItem(endColor, morphTime, startDelay));
            return timeline;
        }

        LFXE_API LightTimeline LightTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startDelay) {
            return LightTimeline::NewPulse(startColor, endColor, pulseTime, pulseAmount, 0, 0, startDelay);
        }

        LFXE_API LightTimeline LightTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const unsigned long startDelay) {
            return LightTimeline::NewPulse(startColor, endColor, startColor, pulseTime, pulseAmount, 0, 0, startDelay);
        }

        LFXE_API LightTimeline LightTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const LightColor& resetColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const unsigned long startDelay) {
            LightTimelineItem actionStart(startColor, 0, 0);

            LightTimelineItem actionStartToEndColorFirst(endColor, pulseTime, startDelay + startColorHoldTime / 2);
            LightTimelineItem actionStartToEndColorOther(endColor, pulseTime, startColorHoldTime);
            LightTimelineItem actionEndToStartColor(startColor, pulseTime, endColorHoldTime);

            LightTimelineItem actionReset(resetColor, 0, startColorHoldTime / 2);

            LightTimeline timeline;
            timeline.AddItem(actionStart);
            for (unsigned long i = 0; i < pulseAmount; ++i) {
                if (i == 0) {
                    timeline.AddItem(actionStartToEndColorFirst);
                } else {
                    timeline.AddItem(actionStartToEndColorOther);
                }
                timeline.AddItem(actionEndToStartColor);
            }
            timeline.AddItem(actionReset);
            return timeline;
        }

    }
}

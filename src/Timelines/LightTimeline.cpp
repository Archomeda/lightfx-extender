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

        LFXE_API unsigned long LightTimeline::GetTotalDuration() const {
            return this->totalDuration;
        }

        LFXE_API LightColor LightTimeline::GetColorAtTime(const unsigned long time) const {
            if (this->items.size() == 0) {
                return LightColor();
            }

            LightTimelineItem currentItem = this->items[0];
            unsigned long timeStart = 0;
            long timeLeft = time - currentItem.GetStartDelay();
            size_t currentIndex = 0;
            while (timeLeft > 0 && currentIndex < this->items.size()) {
                currentItem = this->items[currentIndex];
                timeLeft -= currentItem.GetDuration();
                if (timeLeft > 0) {
                    timeStart += currentItem.GetDuration();
                    if (currentIndex > 0) {
                        timeStart += currentItem.GetStartDelay();
                    }

                    ++currentIndex;
                    if (currentIndex < this->items.size()) {
                        currentItem = this->items[currentIndex];
                        timeLeft -= currentItem.GetStartDelay();
                    }
                }
            }

            LightTimelineItem prevItem = {};
            if (currentIndex > 0) {
                prevItem = this->items[currentIndex - 1];
            }
            if (currentIndex < this->items.size()) {
                long timeRelative = time - timeStart - currentItem.GetStartDelay();
                if (timeRelative < 0) {
                    return prevItem.GetColor();
                } else if (timeRelative == 0) {
                    if (currentItem.GetDuration() == 0) {
                        return currentItem.GetColor();
                    } else {
                        return prevItem.GetColor();
                    }
                } else {
                    LightColor colorStart = prevItem.GetColor();
                    LightColor colorEnd = currentItem.GetColor();
                    double progress = (double)timeRelative / currentItem.GetDuration();

                    return LightColor(
                        int(colorStart.red + progress * (colorEnd.red - colorStart.red)),
                        int(colorStart.green + progress * (colorEnd.green - colorStart.green)),
                        int(colorStart.blue + progress * (colorEnd.blue - colorStart.blue)),
                        int(colorStart.brightness + progress * (colorEnd.brightness - colorStart.brightness))
                        );
                }
            } else {
                return prevItem.GetColor();
            }
        }

        LFXE_API wstring LightTimeline::ToString() const {
            wstring result;
            for (size_t i = 0; i < this->items.size(); ++i) {
                if (i != 0) {
                    result += L"->";
                }
                LightTimelineItem item = this->items.at(i);
                result += L"(" + item.ToString() + L")";
            }
            return result;
        }


        LFXE_API LightTimeline LightTimeline::NewInstant(const LightColor& color) {
            LightTimeline timeline;
            timeline.AddItem(LightTimelineItem(color, 0, 0));
            return timeline;
        }

        LFXE_API LightTimeline LightTimeline::NewMorph(const LightColor& startColor, const LightColor& endColor, const unsigned long morphTime, const long startDelay) {
            LightTimeline timeline;
            timeline.AddItem(LightTimelineItem(startColor, 0, 0));
            timeline.AddItem(LightTimelineItem(endColor, morphTime, startDelay));
            return timeline;
        }

        LFXE_API LightTimeline LightTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const long startDelay) {
            return LightTimeline::NewPulse(startColor, endColor, pulseTime, pulseAmount, 0, 0, startDelay);
        }

        LFXE_API LightTimeline LightTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const long startDelay) {
            return LightTimeline::NewPulse(startColor, endColor, startColor, pulseTime, pulseAmount, 0, 0, startDelay);
        }

        LFXE_API LightTimeline LightTimeline::NewPulse(const LightColor& startColor, const LightColor& endColor, const LightColor& resetColor, const unsigned long pulseTime, const unsigned long pulseAmount, const unsigned long startColorHoldTime, const unsigned long endColorHoldTime, const long startDelay) {
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

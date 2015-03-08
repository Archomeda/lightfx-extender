#pragma once

// Standard includes
#include <string>
#include <vector>

// Project includes
#include "LightColor.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace timelines {

        class LFXE_API LightTimelineItem {

        public:
            LightTimelineItem() {}
            LightTimelineItem(const LightColor& color, const unsigned long duration = 1000, const long startDelay = 0)
                : color(color), duration(duration), startDelay(startDelay) {}

            LightColor GetColor();
            unsigned long GetDuration();
            long GetStartDelay();

            void SetColor(const LightColor& color);
            void SetDuration(const unsigned long duration);
            void SetStartDelay(const long startDelay);

            std::wstring ToString();

            friend inline bool operator==(const LightTimelineItem& a, const LightTimelineItem& b) {
                return a.color == b.color && a.startDelay == b.startDelay && a.duration == b.duration;
            }
            friend inline bool operator!=(const LightTimelineItem& a, const LightTimelineItem& b) { return !operator==(a, b); }

        private:
            LightColor color = {};

            long startDelay = 0;
            unsigned long duration = 1000;

        };

    }
}

#pragma warning(pop)

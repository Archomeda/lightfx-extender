#pragma once

// Standard includes
#include <vector>

// API exports
#include "Common/ApiExports.h"

// Project includes
#include "LightAction.h" //temporary


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {

    class LFXE_API LightColorTimelineAction {

    public:
        LightColorTimelineAction() {}
        LightColorTimelineAction(const LightColor& color, const unsigned long duration = 1000, const unsigned long startDelay = 0)
            : color(color), duration(duration), startDelay(startDelay) {}

        LightColor GetColor();
        unsigned long GetDuration();
        unsigned long GetStartDelay();

        void SetColor(const LightColor& color);
        void SetDuration(const unsigned long duration);
        void SetStartDelay(const unsigned long startDelay);

        friend inline bool operator==(const LightColorTimelineAction& a, const LightColorTimelineAction& b) {
            return a.color == b.color && a.startDelay == b.startDelay && a.duration == b.duration;
        }
        friend inline bool operator!=(const LightColorTimelineAction& a, const LightColorTimelineAction& b) { return !operator==(a, b); }

    private:
        LightColor color = {};

        unsigned long startDelay = 0;
        unsigned long duration = 1000;

    };

}

#pragma warning(pop)

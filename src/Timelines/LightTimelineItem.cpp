#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightTimelineItem.h"


using namespace std;

namespace lightfx {
    namespace timelines {

        LFXE_API LightColor LightTimelineItem::GetColor() const {
            return this->color;
        }

        LFXE_API unsigned long LightTimelineItem::GetDuration() const {
            return this->duration;
        }

        LFXE_API long LightTimelineItem::GetStartDelay() const {
            return this->startDelay;
        }

        LFXE_API void LightTimelineItem::SetColor(const LightColor& color) {
            this->color = color;
        }

        LFXE_API void LightTimelineItem::SetDuration(const unsigned long duration) {
            this->duration = duration;
        }

        LFXE_API void LightTimelineItem::SetStartDelay(const long startDelay) {
            this->startDelay = startDelay;
        }

        LFXE_API wstring LightTimelineItem::ToString() const {
            return L"l" + to_wstring(this->duration) + L",d" + to_wstring(this->startDelay) + L",c("
                + to_wstring(this->color.red) + L"," + to_wstring(this->color.green) + L"," + to_wstring(this->color.blue) + L"," + to_wstring(this->color.brightness) + L")";
        }

    }
}

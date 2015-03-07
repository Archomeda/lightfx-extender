#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightColorTimelineAction.h"


using namespace std;

namespace lightfx {

    LFXE_API LightColor LightColorTimelineAction::GetColor() {
        return this->color;
    }

    LFXE_API unsigned long LightColorTimelineAction::GetDuration() {
        return this->duration;
    }

    LFXE_API unsigned long LightColorTimelineAction::GetStartDelay() {
        return this->startDelay;
    }

    LFXE_API void LightColorTimelineAction::SetColor(const LightColor& color) {
        this->color = color;
    }

    LFXE_API void LightColorTimelineAction::SetDuration(const unsigned long duration) {
        this->duration = duration;
    }

    LFXE_API void LightColorTimelineAction::SetStartDelay(const unsigned long startDelay) {
        this->startDelay = startDelay;
    }

}

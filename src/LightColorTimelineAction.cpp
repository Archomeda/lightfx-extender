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

    LFXE_API wstring LightColorTimelineAction::ToString() {
        return L"l" + to_wstring(this->duration) + L",d" + to_wstring(this->startDelay) + L",c("
            + to_wstring(this->color.red) + L"," + to_wstring(this->color.green) + L"," + to_wstring(this->color.blue) + L"," + to_wstring(this->color.brightness) + L")";
    }

}

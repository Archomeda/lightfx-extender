#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightAction.h"


namespace lightfx {

    LFXE_API LightColor LightAction::GetStartColor() {
        return this->startColor;
    }

    LFXE_API LightColor LightAction::GetEndColor() {
        return this->endColor;
    }

    LFXE_API LightActionType LightAction::GetActionType() {
        return this->actionType;
    }

    LFXE_API unsigned int LightAction::GetActionTime() {
        return this->actionTime;
    }

    LFXE_API unsigned int LightAction::GetStartColorHoldTime() {
        return this->startColorHoldTime;
    }

    LFXE_API unsigned int LightAction::GetEndColorHoldTime() {
        return this->endColorHoldTime;
    }

    LFXE_API unsigned int LightAction::GetActionRepeatAmount() {
        return this->actionRepeatAmount;
    }


    LFXE_API void LightAction::SetStartColor(const LightColor& startColor) {
        this->startColor = startColor;
    }

    LFXE_API void LightAction::SetEndColor(const LightColor& endColor) {
        this->endColor = endColor;
    }

    LFXE_API void LightAction::SetActionType(const LightActionType& actionType) {
        this->actionType = actionType;
    }

    LFXE_API void LightAction::SetActionTime(const unsigned int actionTime) {
        this->actionTime = actionTime;
    }

    LFXE_API void LightAction::SetStartColorHoldTime(const unsigned int startColorHoldTime) {
        this->startColorHoldTime = startColorHoldTime;
    }

    LFXE_API void LightAction::SetEndColorHoldTime(const unsigned int endColorHoldTime) {
        this->endColorHoldTime = endColorHoldTime;
    }

    LFXE_API void LightAction::SetActionRepeatAmount(const unsigned int actionRepeatAmount) {
        this->actionRepeatAmount = actionRepeatAmount;
    }

}

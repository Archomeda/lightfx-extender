#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightAction.h"

// Windows includes
#include "Common/Windows.h"
#include <Windows.h>


using namespace std;

namespace lightfx {

    LFXE_API vector<LightColor> LightAction::GetStartColor() {
        return this->startColor;
    }

    LFXE_API LightColor LightAction::GetStartColor(const size_t lightIndex) {
        return this->startColor[lightIndex];
    }

    LFXE_API vector<LightColor> LightAction::GetEndColor() {
        return this->endColor;
    }

    LFXE_API LightColor LightAction::GetEndColor(const size_t lightIndex) {
        return this->endColor[lightIndex];
    }

    LFXE_API vector<LightColor> LightAction::GetResetColor() {
        return this->resetColor;
    }

    LFXE_API LightColor LightAction::GetResetColor(const size_t lightIndex) {
        return this->resetColor[lightIndex];
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
        this->startColor = vector<LightColor>(this->startColor.size(), startColor);
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetStartColor(const size_t lightIndex, const LightColor& startColor) {
        this->startColor[lightIndex] = startColor;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetStartColor(const vector<LightColor>& startColor) {
        this->startColor = startColor;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetEndColor(const LightColor& endColor) {
        this->endColor = vector<LightColor>(this->endColor.size(), endColor);
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetEndColor(const size_t lightIndex, const LightColor& endColor) {
        this->endColor[lightIndex] = endColor;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetEndColor(const vector<LightColor>& endColor) {
        this->endColor = endColor;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetResetColor(const LightColor& resetColor) {
        this->resetColor = vector<LightColor>(this->resetColor.size(), resetColor);
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetResetColor(const size_t lightIndex, const LightColor& resetColor) {
        this->resetColor[lightIndex] = resetColor;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetResetColor(const vector<LightColor>& resetColor) {
        this->resetColor = resetColor;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetActionType(const LightActionType& actionType) {
        this->actionType = actionType;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetActionTime(const unsigned int actionTime) {
        this->actionTime = actionTime;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetStartColorHoldTime(const unsigned int startColorHoldTime) {
        this->startColorHoldTime = startColorHoldTime;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetEndColorHoldTime(const unsigned int endColorHoldTime) {
        this->endColorHoldTime = endColorHoldTime;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API void LightAction::SetActionRepeatAmount(const unsigned int actionRepeatAmount) {
        this->actionRepeatAmount = actionRepeatAmount;
        this->canUpdateCurrentColor = true;
    }

    LFXE_API LightColor LightAction::GetCurrentColor(const size_t lightIndex) {
        return this->currentColor[lightIndex];
    }

    LFXE_API vector<LightColor> LightAction::GetCurrentColor() {
        return this->currentColor;
    }

    LFXE_API bool LightAction::CanUpdateCurrentColor() {
        return this->canUpdateCurrentColor;
    }

    LFXE_API bool LightAction::UpdateCurrentColor() {
        if (!this->CanUpdateCurrentColor()) {
            return false;
        }

        unsigned long timePassed = 0;
        bool colorChanged = false;

        if (this->animatedColorStartTime == 0) {
            this->animatedColorStartTime = GetTickCount();
        } else {
            timePassed = GetTickCount() - this->animatedColorStartTime;
        }

        switch (this->actionType) {
        case LightActionType::Morph:
            colorChanged = this->UpdateCurrentColorMorph(timePassed);
            break;

        case LightActionType::Pulse:
            colorChanged = this->UpdateCurrentColorPulse(timePassed);
            break;

        case LightActionType::Instant:
            colorChanged = this->UpdateCurrentColorInstant();
            break;
        }

        return colorChanged;
    }

    LFXE_API bool LightAction::UpdateCurrentColorMorph(const unsigned long timePassed) {
        bool colorChanged = false;
        double progress = (double)timePassed / this->actionTime;

        for (size_t i = 0; i < this->currentColor.size(); ++i) {
            LightColor newColor;
            if (timePassed >= this->actionTime) {
                // Time exceeded animation time, set to end color
                newColor = LightColor(this->endColor[i]);
                this->canUpdateCurrentColor = false;
            } else {
                newColor.red = this->startColor[i].red + unsigned char(progress * (this->endColor[i].red - this->startColor[i].red));
                newColor.green = this->startColor[i].green + unsigned char(progress * (this->endColor[i].green - this->startColor[i].green));
                newColor.blue = this->startColor[i].blue + unsigned char(progress * (this->endColor[i].blue - this->startColor[i].blue));
                newColor.brightness = this->startColor[i].brightness + unsigned char(progress * (this->endColor[i].brightness - this->startColor[i].brightness));
            }

            if (this->currentColor[i] != newColor) {
                this->currentColor[i] = newColor;
                colorChanged = true;
            }
        }

        return colorChanged;
    }

    LFXE_API bool LightAction::UpdateCurrentColorPulse(const unsigned long timePassed) {
        bool colorChanged = false;
        unsigned int timePhase = 2 * this->actionTime + this->startColorHoldTime + this->endColorHoldTime;
        unsigned int timeTotal = timePhase * this->actionRepeatAmount;
        unsigned int progress = timePassed % timePhase;

        unsigned int progressPhaseStartColor1 = 0;
        unsigned int progressPhaseTransitionToEnd = this->startColorHoldTime / 2;
        unsigned int progressPhaseEndColor = progressPhaseTransitionToEnd + this->actionTime;
        unsigned int progressPhaseTransitionToStart = progressPhaseEndColor + this->endColorHoldTime;
        unsigned int progressPhaseStartColor2 = progressPhaseTransitionToStart + this->actionTime;

        for (size_t i = 0; i < this->currentColor.size(); ++i) {
            LightColor newColor;
            if (timePassed >= timeTotal) {
                // Time exceeded animation time, reset to prev color
                newColor = LightColor(this->resetColor[i]);
                this->canUpdateCurrentColor = false;
            } else {
                if (progress < progressPhaseTransitionToEnd) {
                    // Phase hold start color 1
                    newColor = LightColor(this->startColor[i]);
                } else if (progress < progressPhaseEndColor) {
                    // Phase transition to end color
                    double transitionProgress = double(progress - progressPhaseTransitionToEnd) / (progressPhaseEndColor - progressPhaseTransitionToEnd);
                    newColor.red = this->startColor[i].red + unsigned char(transitionProgress * (this->endColor[i].red - this->startColor[i].red));
                    newColor.green = this->startColor[i].green + unsigned char(transitionProgress * (this->endColor[i].green - this->startColor[i].green));
                    newColor.blue = this->startColor[i].blue + unsigned char(transitionProgress * (this->endColor[i].blue - this->startColor[i].blue));
                    newColor.brightness = this->startColor[i].brightness + unsigned char(transitionProgress * (this->endColor[i].brightness - this->startColor[i].brightness));
                } else if (progress < progressPhaseTransitionToStart) {
                    // Phase hold end color
                    newColor = LightColor(this->endColor[i]);
                } else if (progress < progressPhaseStartColor2) {
                    // Phase transition to start color
                    double transitionProgress = double(progress - progressPhaseTransitionToStart) / (progressPhaseStartColor2 - progressPhaseTransitionToStart);
                    newColor.red = this->endColor[i].red - unsigned char(transitionProgress * (this->endColor[i].red - this->startColor[i].red));
                    newColor.green = this->endColor[i].green - unsigned char(transitionProgress * (this->endColor[i].green - this->startColor[i].green));
                    newColor.blue = this->endColor[i].blue - unsigned char(transitionProgress * (this->endColor[i].blue - this->startColor[i].blue));
                    newColor.brightness = this->endColor[i].brightness - unsigned char(transitionProgress * (this->endColor[i].brightness - this->startColor[i].brightness));
                } else {
                    // Phase hold start color 2
                    newColor = LightColor(this->startColor[i]);
                }
            }

            if (this->currentColor[i] != newColor) {
                this->currentColor[i] = newColor;
                colorChanged = true;
            }
        }

        return colorChanged;
    }

    LFXE_API bool LightAction::UpdateCurrentColorInstant() {
        bool colorChanged = false;
        for (size_t i = 0; i < this->currentColor.size(); ++i) {
            colorChanged = this->currentColor[i] != this->endColor[i];
            if (colorChanged) {
                break;
            }
        }
        this->currentColor = vector<LightColor>(this->endColor);
        this->canUpdateCurrentColor = false;
        return colorChanged;
    }

}

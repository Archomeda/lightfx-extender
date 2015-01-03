#include "Device.h"

using namespace std;

namespace lightfx {

    bool Device::Initialized() {
        return this->IsInitialized;
    }

    LFX_RESULT Device::Initialize() {
        if (!this->Initialized()) {
            this->IsInitialized = true;
            this->Reset();
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::Release() {
        if (this->Initialized()) {
            this->IsInitialized = false;
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::Reset() {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        this->CurrentPrimaryColor = vector<LFX_COLOR>(this->Lights.size());
        this->NextPrimaryColor = vector<LFX_COLOR>(this->Lights.size());
        this->CurrentSecondaryColor = vector<LFX_COLOR>(this->Lights.size());
        this->NextSecondaryColor = vector<LFX_COLOR>(this->Lights.size());
        this->CurrentAction = vector<int>(this->Lights.size());
        this->NextAction = vector<int>(this->Lights.size());
        this->CurrentTiming = 200;
        this->NextTiming = 200;
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::Update() {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        for (size_t i = 0; i < this->Lights.size(); ++i) {
            this->CurrentPrimaryColor[i] = this->NextPrimaryColor[i];
            this->CurrentSecondaryColor[i] = this->NextSecondaryColor[i];
            this->CurrentAction[i] = this->NextAction[i];
        }
        return LFX_SUCCESS;
    }


    LFX_RESULT Device::GetNumLights(unsigned int& numLights) {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        numLights = this->Lights.size();
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::GetLightDescription(const unsigned int index, string& description) {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (index > this->Lights.size()) {
            return LFX_ERROR_NOLIGHTS;
        }

        description = string(this->Lights[index].Description);
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::GetLightLocation(const unsigned int index, LFX_POSITION& location) {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (index > this->Lights.size()) {
            return LFX_ERROR_NOLIGHTS;
        }

        location = LFX_POSITION(this->Lights[index].Position);
        return LFX_SUCCESS;
    }


    LFX_RESULT Device::GetLightColor(const unsigned int index, LFX_COLOR& color) {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (index > this->Lights.size()) {
            return LFX_ERROR_NOLIGHTS;
        }

        color = LFX_COLOR(this->CurrentPrimaryColor[index]);
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::SetLightColor(const unsigned int index, const LFX_COLOR& color) {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (index > this->Lights.size()) {
            return LFX_ERROR_NOLIGHTS;
        }

        this->NextPrimaryColor[index] = LFX_COLOR(color);
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::Light(const unsigned int locationMask, const LFX_COLOR& color) {
        // locationMask is ignored for now, reserved for possible future use

        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        for (size_t i = 0; i < this->Lights.size(); ++i) {
            this->NextPrimaryColor[i] = LFX_COLOR(color);
        }
        return LFX_SUCCESS;
    }


    LFX_RESULT Device::SetLightActionColor(const unsigned int index, const unsigned int actionType, const LFX_COLOR& primaryColor) {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (index > this->Lights.size()) {
            return LFX_ERROR_NOLIGHTS;
        }

        return this->SetLightActionColor(index, actionType, primaryColor, LFX_COLOR());
    }

    LFX_RESULT Device::SetLightActionColor(const unsigned int index, const unsigned int actionType, const LFX_COLOR& primaryColor, const LFX_COLOR& secondaryColor) {
        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        if (index > this->Lights.size()) {
            return LFX_ERROR_NOLIGHTS;
        }

        this->NextAction[index] = actionType;
        this->NextPrimaryColor[index] = LFX_COLOR(primaryColor);
        if (actionType == LFX_ACTION_MORPH) {
            this->NextSecondaryColor[index] = LFX_COLOR(secondaryColor);
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT Device::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryColor) {
        // locationMask is ignored for now, reserved for possible future use

        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        return this->ActionColor(locationMask, actionType, primaryColor, LFX_COLOR());
    }

    LFX_RESULT Device::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryColor, const LFX_COLOR& secondaryColor) {
        // locationMask is ignored for now, reserved for possible future use

        if (!this->Initialized()) {
            return LFX_ERROR_NOINIT;
        }

        for (size_t i = 0; i < this->Lights.size(); ++i) {
            this->NextAction[i] = actionType;
            this->NextPrimaryColor[i] = LFX_COLOR(primaryColor);
            if (actionType == LFX_ACTION_MORPH) {
                this->NextSecondaryColor[i] = LFX_COLOR(secondaryColor);
            }
        }
        return LFX_SUCCESS;
    }


    LFX_RESULT Device::GetTiming(int& timing) {
        timing = this->CurrentTiming;
        return LFX_SUCCESS;
    }

    LFX_RESULT Device::SetTiming(const int newTiming) {
        this->CurrentTiming = newTiming;
        return LFX_SUCCESS;
    }

}

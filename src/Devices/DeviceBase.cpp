#include "DeviceBase.h"


using namespace std;

namespace lightfx {
    namespace devices {

        DeviceBase::DeviceBase() {
            this->Reset();
        }

        bool DeviceBase::EnableDevice() {
            if (!this->IsEnabled()) {
                if (this->Initialize() == LFX_SUCCESS) {
                    this->IsEnabled_ = true;
                    return true;
                }
                return false;
            }
            return true;
        }
    
        bool DeviceBase::DisableDevice() {
            if (this->IsEnabled()) {
                if (this->Release() == LFX_SUCCESS) {
                    this->IsEnabled_ = false;
                    return true;
                }
                return false;
            }
            return true;
        }


        LFX_RESULT DeviceBase::Initialize() {
            if (!this->IsInitialized()) {
                this->IsInitialized_ = true;
                this->Reset();
            }
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceBase::Release() {
            if (this->IsInitialized()) {
                this->IsInitialized_ = false;
            }
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceBase::Reset() {
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

        LFX_RESULT DeviceBase::Update() {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->CurrentPrimaryColor[i] = this->NextPrimaryColor[i];
                this->CurrentSecondaryColor[i] = this->NextSecondaryColor[i];
                this->CurrentAction[i] = this->NextAction[i];
            }
            return LFX_SUCCESS;
        }


        LFX_RESULT DeviceBase::GetNumLights(unsigned int& numLights) {
            numLights = this->Lights.size();
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceBase::GetLightDescription(const unsigned int index, string& description) {
            if (index > this->Lights.size()) {
                return LFX_ERROR_NOLIGHTS;
            }

            description = string(this->Lights[index].Description);
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceBase::GetLightLocation(const unsigned int index, LFX_POSITION& location) {
            if (index > this->Lights.size()) {
                return LFX_ERROR_NOLIGHTS;
            }

            location = LFX_POSITION(this->Lights[index].Position);
            return LFX_SUCCESS;
        }


        LFX_RESULT DeviceBase::GetLightColor(const unsigned int index, LFX_COLOR& color) {
            if (index > this->Lights.size()) {
                return LFX_ERROR_NOLIGHTS;
            }

            color = LFX_COLOR(this->CurrentPrimaryColor[index]);
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceBase::SetLightColor(const unsigned int index, const LFX_COLOR& color) {
            if (index > this->Lights.size()) {
                return LFX_ERROR_NOLIGHTS;
            }

            this->NextPrimaryColor[index] = LFX_COLOR(color);
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceBase::Light(const unsigned int locationMask, const LFX_COLOR& color) {
            // locationMask is ignored for now, reserved for possible future use
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextPrimaryColor[i] = LFX_COLOR(color);
            }
            return LFX_SUCCESS;
        }


        LFX_RESULT DeviceBase::SetLightActionColor(const unsigned int index, const unsigned int actionType, const LFX_COLOR& primaryColor) {
            if (index > this->Lights.size()) {
                return LFX_ERROR_NOLIGHTS;
            }

            return this->SetLightActionColor(index, actionType, primaryColor, LFX_COLOR());
        }

        LFX_RESULT DeviceBase::SetLightActionColor(const unsigned int index, const unsigned int actionType, const LFX_COLOR& primaryColor, const LFX_COLOR& secondaryColor) {
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

        LFX_RESULT DeviceBase::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryColor) {
            // locationMask is ignored for now, reserved for possible future use
            return this->ActionColor(locationMask, actionType, primaryColor, LFX_COLOR());
        }

        LFX_RESULT DeviceBase::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryColor, const LFX_COLOR& secondaryColor) {
            // locationMask is ignored for now, reserved for possible future use
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextAction[i] = actionType;
                this->NextPrimaryColor[i] = LFX_COLOR(primaryColor);
                if (actionType == LFX_ACTION_MORPH) {
                    this->NextSecondaryColor[i] = LFX_COLOR(secondaryColor);
                }
            }
            return LFX_SUCCESS;
        }


        LFX_RESULT DeviceBase::GetTiming(int& timing) {
            timing = this->CurrentTiming;
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceBase::SetTiming(const int newTiming) {
            this->CurrentTiming = newTiming;
            return LFX_SUCCESS;
        }

    }

}

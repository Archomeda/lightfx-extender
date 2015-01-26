#include "DeviceBase.h"

// Project includes
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {
    namespace devices {

        DeviceBase::DeviceBase() {
            this->Reset();
        }

        bool DeviceBase::EnableDevice() {
            if (!this->IsEnabled()) {
                if (this->Initialize()) {
                    Log(this->GetDeviceName() + L" enabled");
                    this->IsEnabled_ = true;
                    return true;
                }
                return false;
            }
            return true;
        }

        bool DeviceBase::DisableDevice() {
            if (this->IsEnabled()) {
                if (this->Release()) {
                    Log(this->GetDeviceName() + L" disabled");
                    this->IsEnabled_ = false;
                    return true;
                }
                return false;
            }
            return true;
        }


        bool DeviceBase::Initialize() {
            if (!this->IsInitialized()) {
                this->IsInitialized_ = true;
                this->Reset();
            }
            return true;
        }

        bool DeviceBase::Release() {
            if (this->IsInitialized()) {
                this->IsInitialized_ = false;
            }
            return true;
        }

        bool DeviceBase::Update() {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->CurrentPrimaryColor[i] = this->NextPrimaryColor[i];
            }
            return true;
        }

        bool DeviceBase::Reset() {
            this->CurrentPrimaryColor = vector<LFX_COLOR>(this->Lights.size());
            this->NextPrimaryColor = vector<LFX_COLOR>(this->Lights.size());
            return true;
        }


        size_t DeviceBase::GetNumberOfLights() {
            return this->Lights.size();
        }

        DeviceLight DeviceBase::GetLight(const size_t index) {
            if (index < this->Lights.size()) {
                return this->Lights[index];
            }
            return DeviceLight();
        }

        LFX_COLOR DeviceBase::GetPrimaryColorForLight(const size_t index) {
            if (index < this->Lights.size()) {
                return LFX_COLOR(this->CurrentPrimaryColor[index]);
            }
            return LFX_COLOR();
        }

        bool DeviceBase::SetPrimaryColor(const LFX_COLOR& color) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextPrimaryColor[i] = LFX_COLOR(color);
            }
            return true;
        }

        bool DeviceBase::SetPrimaryColorForLight(const size_t index, const LFX_COLOR& color) {
            if (index < this->Lights.size()) {
                this->NextPrimaryColor[index] = LFX_COLOR(color);
            }
            return true;
        }

        bool DeviceBase::SetPrimaryColorForLocation(const LightLocationMask locationMask, const LFX_COLOR& color) {
            // TODO: Implement locationMask filter
            return this->SetPrimaryColor(color);
        }

    }
}

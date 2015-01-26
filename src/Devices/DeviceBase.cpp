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
                this->CurrentColor[i] = this->NextColor[i];
            }
            return true;
        }

        bool DeviceBase::Reset() {
            this->CurrentColor = vector<LFX_COLOR>(this->Lights.size());
            this->NextColor = vector<LFX_COLOR>(this->Lights.size());
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

        LFX_COLOR DeviceBase::GetColorForLight(const size_t index) {
            if (index < this->Lights.size()) {
                return LFX_COLOR(this->CurrentColor[index]);
            }
            return LFX_COLOR();
        }

        bool DeviceBase::SetColor(const LFX_COLOR& color) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextColor[i] = LFX_COLOR(color);
            }
            return true;
        }

        bool DeviceBase::SetColorForLight(const size_t index, const LFX_COLOR& color) {
            if (index < this->Lights.size()) {
                this->NextColor[index] = LFX_COLOR(color);
            }
            return true;
        }

        bool DeviceBase::SetColorForLocation(const LightLocationMask locationMask, const LFX_COLOR& color) {
            // TODO: Implement locationMask filter
            return this->SetColor(color);
        }

    }
}

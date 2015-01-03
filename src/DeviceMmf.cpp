#include "DeviceMmf.h"
#include "LinkedMemAPI.h"
#include "Log.h"

using namespace std;

namespace lightfx {

    DeviceMmf::DeviceMmf(const string& description, const unsigned char type, const vector<DeviceLightData>& lights) {
        this->description = description;
        this->type = type;
        this->Lights = vector<DeviceLightData>(lights);
    }

    LFX_RESULT DeviceMmf::Initialize() {
        if (!this->Initialized()) {
            if (api::linkedMemHandle == nullptr || !api::linkedMemHandle->isInitialized) {
                api::init();
                api::linkedMemHandle->isInitialized = true;
            }

            // TODO: support for multiple MMF devices and lights
            if (this->Lights.size() > 0) {
                api::linkedMemHandle->light.position = LFX_POSITION(this->Lights[0].Position);
            }
            api::linkedMemHandle->tick++;

            return Device::Initialize();
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT DeviceMmf::Release() {
        if (this->Initialized()) {
            if (api::linkedMemHandle->isInitialized) {
                api::linkedMemHandle->isInitialized = false;
                api::linkedMemHandle->tick++;
                api::dispose();
            }

            return Device::Release();
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT DeviceMmf::Update() {
        auto result = Device::Update();
        
        if (result != LFX_SUCCESS) {
            return result;
        }

        // TODO: support for multiple MMF devices and lights
        if (this->Lights.size() > 0) {
            api::linkedMemHandle->light.primaryColor = LFX_COLOR(this->CurrentPrimaryColor[0]);
            api::linkedMemHandle->light.secondaryColor = LFX_COLOR(this->CurrentSecondaryColor[0]);
            api::linkedMemHandle->light.actionType = this->CurrentAction[0];
            api::linkedMemHandle->tick++;
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT DeviceMmf::GetDeviceInfo(string& description, unsigned char& type) {
        description = this->description;
        type = this->type;
        return LFX_SUCCESS;
    }

}

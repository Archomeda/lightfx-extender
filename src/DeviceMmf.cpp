#include "DeviceMmf.h"
#include "Log.h"

using namespace std;
using namespace lightfx::mmf;

namespace lightfx {

    MemoryMappedFileApi mmf::MemoryMappedFile(MMFNAME);

    DeviceMmf::DeviceMmf(const string& description, const unsigned char type, const vector<DeviceLightData>& lights) {
        this->description = description;
        this->type = type;
        this->Lights = vector<DeviceLightData>(lights);
    }

    LFX_RESULT DeviceMmf::Initialize() {
        if (!this->Initialized()) {
            if (!MemoryMappedFile.Opened()) {
                MemoryMappedFile.Open();
                MemoryMappedFile.Handle->isInitialized = true;
            }

            // TODO: support for multiple MMF devices and lights
            if (this->Lights.size() > 0) {
                MemoryMappedFile.Handle->light.position = LFX_POSITION(this->Lights[0].Position);
            }
            MemoryMappedFile.Handle->timing = this->CurrentTiming;
            MemoryMappedFile.Handle->tick++;

            return Device::Initialize();
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT DeviceMmf::Release() {
        if (this->Initialized()) {
            if (MemoryMappedFile.Opened()) {
                MemoryMappedFile.Handle->isInitialized = false;
                MemoryMappedFile.Handle->tick++;
                MemoryMappedFile.Close();
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

        MemoryMappedFile.Handle->timing = this->CurrentTiming;

        // TODO: support for multiple MMF devices and lights
        if (this->Lights.size() > 0) {
            MemoryMappedFile.Handle->light.primaryColor = LFX_COLOR(this->CurrentPrimaryColor[0]);
            MemoryMappedFile.Handle->light.secondaryColor = LFX_COLOR(this->CurrentSecondaryColor[0]);
            MemoryMappedFile.Handle->light.actionType = this->CurrentAction[0];
            MemoryMappedFile.Handle->tick++;
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT DeviceMmf::GetDeviceInfo(string& description, unsigned char& type) {
        description = this->description;
        type = this->type;
        return LFX_SUCCESS;
    }

}

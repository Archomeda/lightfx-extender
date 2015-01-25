#include "DeviceMmf.h"


using namespace std;
using namespace lightfx::devices::mmf;

namespace lightfx {
    namespace devices {

        MemoryMappedFileApi mmf::MemoryMappedFile(MMFNAME);

        DeviceMmf::DeviceMmf(const string& description, const unsigned char type, const vector<DeviceLightData>& lights) : DeviceBase() {
            this->description = description;
            this->type = type;
            this->Lights = vector<DeviceLightData>(lights);
            this->Reset();
        }

        LFX_RESULT DeviceMmf::Initialize() {
            if (!this->IsInitialized()) {
                if (!MemoryMappedFile.Opened()) {
                    MemoryMappedFile.Open();
                    MemoryMappedFile.Handle->numDevices = 0;
                    MemoryMappedFile.Handle->isInitialized = true;
                }

                this->mmfIndex = MemoryMappedFile.Handle->numDevices;
                strcpy_s(MemoryMappedFile.Handle->devices[this->mmfIndex].name, this->description.c_str());
                MemoryMappedFile.Handle->devices[this->mmfIndex].numLights = this->Lights.size();
                MemoryMappedFile.Handle->devices[this->mmfIndex].type = this->type;
                for (size_t i = 0; i < this->Lights.size(); ++i) {
                    MemoryMappedFile.Handle->devices[this->mmfIndex].lights[i].position = LFX_POSITION(this->Lights[i].Position);
                    strcpy_s(MemoryMappedFile.Handle->devices[this->mmfIndex].lights[i].name, this->Lights[i].Description.c_str());
                }
                MemoryMappedFile.Handle->numDevices++;

                MemoryMappedFile.Handle->timing = this->CurrentTiming;
                MemoryMappedFile.Handle->tick++;

                return DeviceBase::Initialize();
            }
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceMmf::Release() {
            if (this->IsInitialized()) {
                if (MemoryMappedFile.Opened()) {
                    MemoryMappedFile.Handle->isInitialized = false;
                    MemoryMappedFile.Handle->tick++;
                    MemoryMappedFile.Close();
                }

                return DeviceBase::Release();
            }
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceMmf::Update() {
            if (!this->IsEnabled()) {
                return LFX_SUCCESS;
            }

            auto result = DeviceBase::Update();
            if (result != LFX_SUCCESS) {
                return result;
            }

            MemoryMappedFile.Handle->timing = this->CurrentTiming;

            for (size_t i = 0; i < this->Lights.size(); ++i) {
                MemoryMappedFile.Handle->devices[this->mmfIndex].lights[i].primaryColor = LFX_COLOR(this->CurrentPrimaryColor[i]);
                MemoryMappedFile.Handle->devices[this->mmfIndex].lights[i].secondaryColor = LFX_COLOR(this->CurrentSecondaryColor[i]);
                MemoryMappedFile.Handle->devices[this->mmfIndex].lights[i].actionType = this->CurrentAction[i];
            }
            if (this->Lights.size() > 0) {
                // Check if this MMF device is the last one to update, otherwise don't increment tick (this is a bit of a workaround)
                if (this->mmfIndex + 1 == MemoryMappedFile.Handle->numDevices) {
                    MemoryMappedFile.Handle->tick++;
                }
            }
            return LFX_SUCCESS;
        }

        LFX_RESULT DeviceMmf::GetDeviceInfo(string& description, unsigned char& type) {
            description = this->description;
            type = this->type;
            return LFX_SUCCESS;
        }

    }
}

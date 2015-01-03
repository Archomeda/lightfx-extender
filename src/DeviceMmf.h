#pragma once
#include <memory>
#include "Device.h"
#include "MemoryMappedFileApi.h"

#define MMFNAME "AlienFXFrameworkLink"

namespace lightfx {

    namespace mmf {
        extern MemoryMappedFileApi MemoryMappedFile;
    }

    class DeviceMmf : public Device {

    public:
        DeviceMmf(const std::string& description, const unsigned char type, const std::vector<DeviceLightData>& lights);

        virtual LFX_RESULT Initialize() override;
        virtual LFX_RESULT Release() override;
        virtual LFX_RESULT Update() override;

        virtual LFX_RESULT GetDeviceInfo(std::string& description, unsigned char& type) override;

    private:
        std::string description;
        unsigned char type;
        size_t MmfIndex = 0; // Reserved for future support for multiple MMF devices

    };

}

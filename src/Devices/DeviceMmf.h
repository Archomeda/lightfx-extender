#pragma once

// Standard includes
#include <memory>

// Project includes
#include "../MemoryMappedFileApi.h"
#include "DeviceBase.h"


#define MMFNAME "AlienFXFrameworkLink"

namespace lightfx {
    namespace devices {

        namespace mmf {
            extern MemoryMappedFileApi MemoryMappedFile;
        }

        class DeviceMmf : public DeviceBase {

        public:
            DeviceMmf(const std::string& description, const unsigned char type, const std::vector<DeviceLightData>& lights);

            virtual LFX_RESULT Initialize() override;
            virtual LFX_RESULT Release() override;
            virtual LFX_RESULT Update() override;

            virtual LFX_RESULT GetDeviceInfo(std::string& description, unsigned char& type) override;

        private:
            std::string description;
            unsigned char type;
            size_t mmfIndex = 0;

        };

    }
}

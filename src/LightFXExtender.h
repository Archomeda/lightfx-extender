#pragma once

// Standard includes
#include <memory>

// API exports
#include "Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class ConfigManager;
        class DeviceManager;
    }

    class LFXE_API LightFXExtender : public std::enable_shared_from_this < LightFXExtender >  {

    public:
        std::shared_ptr<managers::ConfigManager> GetConfigManager();
        std::shared_ptr<managers::DeviceManager> GetDeviceManager();


    private:
        std::weak_ptr<managers::ConfigManager> configManager;
        std::weak_ptr<managers::DeviceManager> deviceManager;

    };

}

#pragma warning(pop)

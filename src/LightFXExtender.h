#pragma once

// Standard includes
#include <memory>

// Project includes
#include "Managers/ConfigManager.h"
#include "Managers/DeviceManager.h"
#include "Managers/GameManager.h"
#include "Managers/LogManager.h"
#include "Managers/TrayManager.h"
#include "Managers/UpdateManager.h"

// API exports
#include "Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class ConfigManager;
    }

    class LFXE_API LightFXExtender : public std::enable_shared_from_this < LightFXExtender >  {

    public:
        std::shared_ptr<managers::ConfigManager> GetConfigManager();


    private:
        std::weak_ptr<managers::ConfigManager> configManager;

    };

}

#pragma warning(pop)

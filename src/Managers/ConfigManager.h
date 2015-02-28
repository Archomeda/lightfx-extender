#pragma once

// READ FIRST
// If you want to use ConfigManager::GetMainConfig, include MainConfigFile.h in your header


// Project includes
#include "ManagerWithChildren.h"
#include "../Config/ConfigFile.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace config {
        class ConfigFile;
        class MainConfigFile;
    }

    namespace managers {

        class LFXE_API ConfigManager : public ManagerWithChildren < config::ConfigFile, ConfigManager > {

        public:
            ConfigManager() {};

            bool InitializeConfigs();

            std::shared_ptr<config::MainConfigFile> GetMainConfig();

        };

    }
}

#pragma warning(pop)

#pragma once

// Project includes
#include "Manager.h"
#include "../Config/ConfigFile.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace config {
        class ConfigFile;
    }

    namespace managers {

        class LFXE_API ConfigManager : public Manager < config::ConfigFile, ConfigManager > {

        public:
            ConfigManager() {};

        };

        template class LFXE_API Manager < config::ConfigFile, ConfigManager > ;

    }
}

#pragma once

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
    }

    namespace managers {

        class LFXE_API ConfigManager : public ManagerWithChildren < config::ConfigFile, ConfigManager > {

        public:
            ConfigManager() {};

        };

    }
}

#pragma warning(pop)

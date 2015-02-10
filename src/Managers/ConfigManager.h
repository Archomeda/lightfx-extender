#pragma once

// Standard includes
#include <string>
#include <map>
#include <memory>

// Project includes
#include "Manager.h"
#include "../LightFXExtender.h"
#include "../Config/ConfigFile.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace config {
        class ConfigFile;
    }

    class LightFXExtender;

    namespace managers {

        template class LFXE_API Manager < config::ConfigFile, ConfigManager > ;

        class LFXE_API ConfigManager : public Manager < config::ConfigFile, ConfigManager > {

        public:
            ConfigManager() {};

        };

    }
}

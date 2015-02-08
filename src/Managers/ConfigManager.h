#pragma once

// Standard includes
#include <string>
#include <map>
#include <memory>

// Project includes
#include "../LightFXExtender.h"
#include "../Config/ConfigFile.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {

    class LightFXExtender;

    namespace managers {

        typedef std::shared_ptr<config::ConfigFile> spConfigFile;
        typedef std::map<std::wstring, spConfigFile> mapConfigFiles;

        class LFXE_API ConfigManager {

        public:
            ConfigManager(const LightFXExtender* lightFXExtender);
            ~ConfigManager();

            void AddConfigFile(const std::wstring& configKey, const spConfigFile& configFile);
            spConfigFile GetConfigFile(const std::wstring& configKey);

        private:
            mapConfigFiles configFiles = {};

        };

    }
}

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


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace config {
        class ConfigFile;
    }

    class LightFXExtender;

    namespace managers {

        typedef std::shared_ptr<config::ConfigFile> spConfigFile;
        typedef std::map<std::wstring, std::weak_ptr<config::ConfigFile>> mapConfigFiles;
        typedef std::shared_ptr<LightFXExtender> spLightFXExtender;

        class LFXE_API ConfigManager : public std::enable_shared_from_this < ConfigManager > {

        public:
            ConfigManager();
            ~ConfigManager();

            spLightFXExtender GetLightFXExtender();
            void SetLightFXExtender(spLightFXExtender lightFXExtender);

            void AddConfigFile(const std::wstring& configKey, spConfigFile configFile);
            spConfigFile GetConfigFile(const std::wstring& configKey);

            const std::wstring GetConfigDirectory();

        private:
            spLightFXExtender lightFXExtender;
            mapConfigFiles configFiles = {};


        };

    }
}

#pragma warning(pop)

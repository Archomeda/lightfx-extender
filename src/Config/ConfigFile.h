#pragma once

// Standard includes
#include <string>

// Project includes
#include "../Managers/ConfigManager.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class ConfigManager;
    }

    namespace config {

        typedef std::shared_ptr<managers::ConfigManager> spConfigManager;
        
        class LFXE_API ConfigFile {

        public:
            ConfigFile();

            spConfigManager GetConfigManager();
            void SetConfigManager(spConfigManager configManager);

            void Load(const std::wstring& fileName);
            void Save();
            void Save(const std::wstring& fileName);

            const std::wstring GetCurrentFileName();
            virtual const std::wstring GetConfigDirectory();
            virtual void SetConfigDirectory(const std::wstring& directory);

        protected:

            virtual void LoadDefaults() = 0;
            virtual std::wstring Serialize() = 0;
            virtual void Deserialize(const std::wstring& data) = 0;

        private:
            spConfigManager configManager;
            std::wstring configFileName;
            std::wstring configDirectory;

        };

    }
}

#pragma warning(pop)

#pragma once

// Standard includes
#include <string>
#include <memory>

// Project includes
#include "../Managers/ConfigManager.h"
#include "../Managers/ChildOfManager.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class ConfigManager;
    }

    namespace config {

        class LFXE_API ConfigFile : public managers::ChildOfManager < managers::ConfigManager > {

        public:
            ConfigFile();

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
            std::wstring configFileName;
            std::wstring configDirectory;

        };

        template class LFXE_API managers::ChildOfManager < managers::ConfigManager >;

    }
}

#pragma warning(pop)

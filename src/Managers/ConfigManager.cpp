#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "ConfigManager.h"

// Project includes
#include "../Config/MainConfigFile.h"
#include "../LightFXExtender.h"
#include "../Utils/Log.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"ConfigManager - ") + message)

using namespace std;
using namespace lightfx::config;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API bool ConfigManager::InitializeConfigs() {
            auto configFile = make_shared<MainConfigFile>();
            this->AddChild(L"MainConfig", configFile);
            configFile->Load();

            LOG(LogLevel::Info, L"Config files loaded");

            return true;
        }

        LFXE_API shared_ptr<MainConfigFile> ConfigManager::GetMainConfig() {
            return dynamic_pointer_cast<MainConfigFile>(this->GetChild(L"MainConfig"));
        }

    }
}

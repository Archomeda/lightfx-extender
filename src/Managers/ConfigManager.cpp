#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "ConfigManager.h"

// Project includes
#include "../Config/MainConfigFile.h"
#include "../LightFXExtender.h"
#include "LogManager.h"


#define LOG(logLevel, line) if (this->GetLightFXExtender() != nullptr) { this->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"ConfigManager - ") + line); }

using namespace std;
using namespace lightfx::config;

namespace lightfx {
    namespace managers {

        LFXE_API bool ConfigManager::InitializeConfigs() {
            LOG(LogLevel::Debug, L"Loading config files");

            auto configFile = make_shared<MainConfigFile>();
            this->AddChild(L"MainConfig", configFile);
            configFile->Load();

            return true;
        }

        LFXE_API shared_ptr<MainConfigFile> ConfigManager::GetMainConfig() {
            return dynamic_pointer_cast<MainConfigFile>(this->GetChild(L"MainConfig"));
        }

    }
}

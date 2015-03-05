#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "ConfigManager.h"

// Project includes
#include "../Config/MainConfigFile.h"


using namespace std;
using namespace lightfx::config;

namespace lightfx {
    namespace managers {

        LFXE_API bool ConfigManager::InitializeConfigs() {
            auto configFile = make_shared<MainConfigFile>();
            configFile->Load();
            this->AddChild(L"MainConfig", configFile);

            return true;
        }

        LFXE_API shared_ptr<MainConfigFile> ConfigManager::GetMainConfig() {
            return dynamic_pointer_cast<MainConfigFile>(this->GetChild(L"MainConfig"));
        }

    }
}

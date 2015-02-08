#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "ConfigManager.h"


using namespace std;
using namespace lightfx::config;

namespace lightfx {
    namespace managers {

        LFXE_API ConfigManager::ConfigManager(const LightFXExtender* lightFXExtender) {

        }

        LFXE_API ConfigManager::~ConfigManager() {

        }


        LFXE_API void ConfigManager::AddConfigFile(const wstring& configKey, const spConfigFile& configFile) {

        }

        LFXE_API spConfigFile ConfigManager::GetConfigFile(const wstring& configKey) {
            return nullptr;
        }

    }
}
#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "ConfigManager.h"


using namespace std;
using namespace lightfx::config;

namespace lightfx {
    namespace managers {

        LFXE_API ConfigManager::ConfigManager() {

        }

        LFXE_API ConfigManager::~ConfigManager() {

        }


        LFXE_API spLightFXExtender ConfigManager::GetLightFXExtender() {
            return nullptr;
        }

        LFXE_API void ConfigManager::SetLightFXExtender(spLightFXExtender lightFXExtender) {
            
        }


        LFXE_API void ConfigManager::AddConfigFile(const wstring& configKey, spConfigFile configFile) {

        }

        LFXE_API spConfigFile ConfigManager::GetConfigFile(const wstring& configKey) {
            return nullptr;
        }

    }
}
#pragma once

// Standard includes
#include <memory>

// API exports
#include "Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class ConfigManager;
        class DeviceManager;
        class GameManager;
        class LogManager;
        class TrayManager;
        class UpdateManager;
    }

    class LFXE_API LightFXExtender : public std::enable_shared_from_this < LightFXExtender >  {

    public:
        LightFXExtender() {}
        void Initialize();

        bool IsInitialized();

        void Start();
        void Stop();

        std::shared_ptr<managers::ConfigManager> GetConfigManager() { return this->configManager; }
        std::shared_ptr<managers::DeviceManager> GetDeviceManager() { return this->deviceManager; }
        std::shared_ptr<managers::GameManager> GetGameManager() { return this->gameManager; }
        std::shared_ptr<managers::LogManager> GetLogManager() { return this->logManager; }
        std::shared_ptr<managers::TrayManager> GetTrayManager() { return this->trayManager; }
        std::shared_ptr<managers::UpdateManager> GetUpdateManager() { return this->updateManager; }

    private:
        bool isInitialized;

        std::shared_ptr<managers::ConfigManager> configManager;
        std::shared_ptr<managers::DeviceManager> deviceManager;
        std::shared_ptr<managers::GameManager> gameManager;
        std::shared_ptr<managers::LogManager> logManager;
        std::shared_ptr<managers::TrayManager> trayManager;
        std::shared_ptr<managers::UpdateManager> updateManager;

    };

}

#pragma warning(pop)

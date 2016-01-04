#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LightFXExtender.h"

// Project includes
#include "Managers/ConfigManager.h"
#include "Managers/DeviceManager.h"
#include "Managers/GameManager.h"
#include "Managers/TrayManager.h"
#include "Managers/UpdateManager.h"
#include "Config/MainConfigFile.h"
#include "Utils/Log.h"
#include "Utils/Windows.h"


using namespace std;
using namespace lightfx::config;
using namespace lightfx::managers;
using namespace lightfx::utils;

namespace lightfx {

    LFXE_API void LightFXExtender::Initialize() {
        if (!this->isInitialized) {
            shared_ptr<ConfigManager> configMgr = make_shared<ConfigManager>();
            shared_ptr<DeviceManager> deviceMgr = make_shared<DeviceManager>();
            shared_ptr<GameManager> gameMgr = make_shared<GameManager>();
            shared_ptr<TrayManager> trayMgr = make_shared<TrayManager>();
            shared_ptr<UpdateManager> updateMgr = make_shared<UpdateManager>();

            configMgr->SetLightFXExtender(shared_from_this());
            deviceMgr->SetLightFXExtender(shared_from_this());
            gameMgr->SetLightFXExtender(shared_from_this());
            trayMgr->SetLightFXExtender(shared_from_this());
            updateMgr->SetLightFXExtender(shared_from_this());

            this->configManager = configMgr;
            this->deviceManager = deviceMgr;
            this->gameManager = gameMgr;
            this->trayManager = trayMgr;
            this->updateManager = updateMgr;

            this->isInitialized = true;
        }
    }

    LFXE_API bool LightFXExtender::IsInitialized() {
        return this->isInitialized;
    }

    LFXE_API void LightFXExtender::Start() {
        Log::StartLoggerWorker();

        Log::RotateLog();
        LOG_INFO(L"LightFX Extender v" + this->updateManager->GetCurrentVersion().ToString());

        wstring processFileName;
        wstring ext;
        wstring processPath = GetProcessName(nullptr, nullptr, &processFileName, &ext);
        processFileName = processFileName + ext;

        LOG_INFO(L"Connected to " + processPath);

        this->configManager->InitializeConfigs();
        Log::SetMinimumLogLevel(this->configManager->GetMainConfig()->MinimumLogLevel);
        this->deviceManager->InitializeDevices();
        this->gameManager->InitializeGame(processFileName);
        this->deviceManager->StartUpdateDevicesWorker();

        if (this->configManager->GetMainConfig()->TrayIconEnabled) {
            this->trayManager->AddTrayIcon();
        }

        this->updateManager->CheckAsync();
    }

    LFXE_API void LightFXExtender::Stop() {
        if (this->configManager->GetMainConfig()->TrayIconEnabled) {
            this->trayManager->RemoveTrayIcon();
        }

        this->configManager->GetMainConfig()->Save();

        this->gameManager->UninitializeGames();
        this->deviceManager->StopUpdateDevicesWorker();
        this->deviceManager->UninitializeDevices();

        Log::StopLoggerWorker();
    }

}

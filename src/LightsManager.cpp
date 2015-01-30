#include "LightsManager.h"

// Project includes
#include "Devices/DeviceLogitech.h"
#include "Devices/DeviceLightpack.h"
#include "Games/Guildwars2.h"
#include "Utils/Log.h"
#include "Utils/String.h"
#include "Utils/Windows.h"


using namespace std;
using namespace lightfx::devices;
using namespace lightfx::games;
using namespace lightfx::utils;

namespace lightfx {

    bool LightsManager::IsInitialized() {
        return this->isInitialized;
    }

    bool LightsManager::HasDevices() {
        return this->GetNumberOfDevices() > 0;
    }

    size_t LightsManager::Initialize() {
        Log(L"Connected to " + GetProcessName());

        // Load config
        this->config.Load();

        // Start checking for updates
        this->updateCheckerThread = thread(&LightsManager::CheckForUpdate, this);

        // Add devices
        this->AddDevices();

        // Load games
        this->AddGames();

        // Add tray icon
        this->trayIcon = TrayIcon();
        this->trayIcon.AddTrayIcon();

        // Initialize devices
        size_t done = 0;
        for (auto device : this->devices) {
            this->trayIcon.AttachDevice(device);

            // Check which devices should be enabled on start
            wstring deviceName = device->GetDeviceName();
            if (this->config.DeviceStates.count(deviceName) > 0) {
                device->EnableDevice();
            }

            ++done;
        }

        // If it's a special supported game, initialize the features
        if (this->currentGame) {
            this->currentGame->Initialize();
            Log(L"Initialized special features for " + this->currentGame->GetName());
        }

        this->isInitialized = true;
        return done;
    }

    size_t LightsManager::Release() {
        // Remove tray icon
        this->trayIcon.RemoveTrayIcon();

        // Save before releasing stuff
        for (auto device : this->devices) {
            this->config.DeviceStates[device->GetDeviceName()] = device->IsEnabled();
        }
        this->config.Save();

        // Release special supported game, if it's one
        if (this->currentGame) {
            this->currentGame->Release();
        }

        // Release devices
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->Release()) {
                ++done;
            }
        }

        // Join update thread in order to close properly
        this->updateCheckerThread.join();

        this->isInitialized = false;
        return done;
    }

    size_t LightsManager::Update() {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->Update()) {
                ++done;
            }
        }
        return done;
    }

    size_t LightsManager::Reset() {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->Reset()) {
                ++done;
            }
        }
        return done;
    }

    size_t LightsManager::GetNumberOfDevices() {
        return this->devices.size();
    }

    shared_ptr<DeviceBase> LightsManager::GetDevice(const size_t index) {
        if (index < this->GetNumberOfDevices()) {
            return this->devices[index];
        }
        return nullptr;
    }

    size_t LightsManager::SetColor(const LFX_COLOR& color) {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->SetColor(color)) {
                ++done;
            }
        }
        return done;
    }

    size_t LightsManager::SetColorForLocation(const LightLocationMask locationMask, const LFX_COLOR& color) {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->SetColorForLocation(locationMask, color)) {
                ++done;
            }
        }
        return done;
    }

    size_t LightsManager::MorphTo(const LFX_COLOR& color, unsigned int time) {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->MorphTo(color, time)) {
                ++done;
            }
        }
        return done;
    }

    size_t LightsManager::MorphToForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, unsigned int time) {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->MorphToForLocation(locationMask, color, time)) {
                ++done;
            }
        }
        return done;
    }

    size_t LightsManager::Pulse(const LFX_COLOR& color, unsigned int time, unsigned int amount) {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->Pulse(color, time, amount)) {
                ++done;
            }
        }
        return true;
    }

    size_t LightsManager::PulseForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, unsigned int time, unsigned int amount) {
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->PulseForLocation(locationMask, color, time, amount)) {
                ++done;
            }
        }
        return true;
    }

    void LightsManager::CheckForUpdate() {
        Version currentVersion = this->updateManager.GetCurrentVersion();
        Log(L"Checking for updates... (current version: " +
            to_wstring(currentVersion.GetMajor()) + L"." + to_wstring(currentVersion.GetMinor()) + L"." + to_wstring(currentVersion.GetBuild()) + L")");

        Version liveVersion = this->updateManager.GetLiveVersion();
        if (liveVersion > currentVersion) {
            wstring newVersionString = to_wstring(liveVersion.GetMajor()) + L"." + to_wstring(liveVersion.GetMinor()) + L"." + to_wstring(liveVersion.GetBuild());
            wstring newVersionUrl = this->updateManager.GetDownloadPageUrl();
            Log(L"A newer version is available: " + newVersionString);
            Log(L"See " + newVersionUrl + L" for downloads");
            this->trayIcon.AddUpdateNotification(newVersionString, newVersionUrl);
        } else {
            Log(L"No update available");
        }
    }


    void LightsManager::AddDevices() {
        this->devices.push_back(shared_ptr<DeviceLogitech>(new DeviceLogitech()));
        this->devices.push_back(shared_ptr<DeviceLightpack>(new DeviceLightpack(this->config.LightpackHost, this->config.LightpackPort, this->config.LightpackKey)));
    }

    void LightsManager::AddGames() {
        this->games.push_back(shared_ptr<Guildwars2>(new Guildwars2()));

        wstring fname, ext;
        GetProcessName(nullptr, nullptr, &fname, &ext);
        wstring fileName = fname + ext;
        for (auto game : this->games) {
            if (game->GetFileName() == fileName) {
                this->currentGame = game;
                break;
            }
        }
    }

}

#include "LightsManager.h"

// Project includes
#include "Devices/DeviceLogitech.h"
#include "Devices/DeviceLightpack.h"
#include "Utils/Log.h"
#include "Utils/String.h"
#include "Utils/Windows.h"


using namespace std;
using namespace lightfx::devices;
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

        // Add devices
        this->AddDevices();

        // Add tray icon
        this->trayIcon = TrayIcon();
        this->trayIcon.AddTrayIcon();

        // Initialize devices
        size_t done = 0;
        for (auto device : this->devices) {
            this->trayIcon.AttachDevice(device);

            // Check which devices should be enabled on start
            wstring deviceName = device->GetDeviceName();
            if (this->config.DeviceStates.count(wstring_to_string(deviceName)) > 0) {
                device->EnableDevice();
            }

            ++done;
        }
        this->isInitialized = true;
        return done;
    }

    size_t LightsManager::Release() {
        // Remove tray icon
        this->trayIcon.RemoveTrayIcon();

        // Save before releasing stuff
        for (auto device : this->devices) {
            this->config.DeviceStates[wstring_to_string(device->GetDeviceName())] = device->IsEnabled();
        }
        this->config.Save();

        // Release devices
        size_t done = 0;
        for (auto device : this->devices) {
            if (device->Release()) {
                ++done;
            }
        }
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

    void LightsManager::AddDevices() {
        this->devices.push_back(shared_ptr<DeviceLogitech>(new DeviceLogitech()));
        this->devices.push_back(shared_ptr<DeviceLightpack>(new DeviceLightpack(this->config.LightpackHost, this->config.LightpackPort, this->config.LightpackKey)));
    }

}

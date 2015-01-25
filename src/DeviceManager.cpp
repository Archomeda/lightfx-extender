#include "DeviceManager.h"

// Standard includes
#include <locale>
#include <codecvt>

// Project includes
#include "Utils/FileIO.h"
#include "Utils/Log.h"
#include "Utils/Windows.h"
#include "resource.h"


using namespace std;
using namespace lightfx::devices;
using namespace lightfx::utils;

namespace lightfx {

    DeviceManager::~DeviceManager() {
        this->Dispose();
        this->lightFXManager.~LightFX();
    }

    LightFX* DeviceManager::GetLightFXManager() {
        return &this->lightFXManager;
    }

    bool DeviceManager::Initialize() {
        if (!this->isInitialized) {
            Log(L"Connected to " + GetProcessName(nullptr, nullptr, nullptr, nullptr));

            // Config
            this->config.Load();

            this->trayIcon = TrayIcon();
            this->trayIcon.AddTrayIcon();

            // Logitech
            this->deviceLogitech = shared_ptr<DeviceLogitech>(new DeviceLogitech());
            this->deviceLogitech->SetRange(this->config.LogitechColorRangeOutMin, this->config.LogitechColorRangeOutMax, this->config.LogitechColorRangeInMin, this->config.LogitechColorRangeInMax);
            this->lightFXManager.AddDevice(this->deviceLogitech);
            this->trayIcon.AttachDevice(this->deviceLogitech);
            if (this->config.LogitechEnabled) {
                this->EnableLogitechDevice();
            }

            // Lightpack
            this->deviceLightpack = shared_ptr<DeviceLightpack>(new DeviceLightpack(this->config.LightpackHost, this->config.LightpackPort, this->config.LightpackKey));
            this->lightFXManager.AddDevice(this->deviceLightpack);
            this->trayIcon.AttachDevice(this->deviceLightpack);
            if (this->config.LightpackEnabled) {
                this->EnableLightpackDevice();
            }

            // Memory mapped file
            for (DeviceData devData : this->config.MemoryMappedFileDevices) {
                auto device = shared_ptr<DeviceMmf>(new DeviceMmf(devData.Description, devData.Type, devData.Lights));
                this->deviceMemoryMappedFile.push_back(device);
                this->lightFXManager.AddDevice(device);
                this->trayIcon.AttachDevice(device);
            }
            if (this->config.MemoryMappedFileEnabled) {
                this->EnableMmfDevice();
            }

            this->isInitialized = true;
        }
        return true;
    }

    bool DeviceManager::Dispose() {
        if (this->isInitialized) {
            this->trayIcon.RemoveTrayIcon();

            // Save before disabling stuff
            this->config.Save();

            this->DisableLogitechDevice();
            this->DisableLightpackDevice();
            this->DisableMmfDevice();

            this->isInitialized = false;
        }
        return true;
    }

    bool DeviceManager::EnableLogitechDevice() {
        if (!this->deviceLogitech->IsEnabled()) {
            this->config.LogitechEnabled = true;
            if (this->deviceLogitech->EnableDevice()) {
                Log("Logitech export enabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::DisableLogitechDevice() {
        if (this->deviceLogitech->IsEnabled()) {
            this->config.LogitechEnabled = false;
            if (this->deviceLogitech->DisableDevice()) {
                Log("Logitech export disabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::EnableLightpackDevice() {
        if (!this->deviceLightpack->IsEnabled()) {
            this->config.LightpackEnabled = true;
            if (this->deviceLightpack->EnableDevice()) {
                Log("Lightpack export enabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::DisableLightpackDevice() {
        if (this->deviceLightpack->IsEnabled()) {
            this->config.LightpackEnabled = false;
            if (this->deviceLightpack->DisableDevice()) {
                Log("Lightpack export disabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::EnableMmfDevice() {
        if (!this->deviceMemoryMappedFileEnabled) {
            this->config.MemoryMappedFileEnabled = true;
            for (auto device : this->deviceMemoryMappedFile) {
                device->EnableDevice();
            }
            Log("Memory Mapped File passthrough enabled");
            this->deviceMemoryMappedFileEnabled = true;
        }
        return true;
    }

    bool DeviceManager::DisableMmfDevice() {
        if (this->deviceMemoryMappedFileEnabled) {
            this->config.MemoryMappedFileEnabled = false;
            for (auto device : this->deviceMemoryMappedFile) {
                device->DisableDevice();
            }
            Log("Memory Mapped File passthrough disabled");
            this->deviceMemoryMappedFileEnabled = false;
        }
        return true;
    }

}

#pragma once

// Standard includes
#include <string>

// Project includes
#include "Devices/DeviceLogitech.h"
#include "Devices/DeviceLightpack.h"
#include "Devices/DeviceMmf.h"
#include "Config.h"
#include "LightFX.h"
#include "TrayIcon.h"


namespace lightfx {

    class DeviceManager {

    public:
        ~DeviceManager();
        LightFX* GetLightFXManager();

        bool Initialize();
        bool Dispose();

        bool EnableLogitechDevice();
        bool DisableLogitechDevice();
        bool EnableLightpackDevice();
        bool DisableLightpackDevice();
        bool EnableMmfDevice();
        bool DisableMmfDevice();

    private:
        Config config;
        LightFX lightFXManager;
        TrayIcon trayIcon;

        bool isInitialized = false;

        std::shared_ptr<devices::DeviceLogitech> deviceLogitech;
        std::shared_ptr<devices::DeviceLightpack> deviceLightpack;
        std::vector<std::shared_ptr<devices::DeviceMmf>> deviceMemoryMappedFile;
        bool deviceMemoryMappedFileEnabled = false;

    };

}

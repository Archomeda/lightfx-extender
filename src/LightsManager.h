#pragma once

// Standard includes
#include <vector>
#include <memory>

// Project includes
#include "Devices/DeviceBase.h"
#include "LightLocationMask.h"
#include "Config.h"
#include "TrayIcon.h"


namespace lightfx {

    class LightsManager {

    public:
        static LightsManager& Instance() {
            static LightsManager instance;
            return instance;
        }

        bool IsInitialized();
        bool HasDevices();

        size_t Initialize();
        size_t Release();
        size_t Update();
        size_t Reset();

        size_t GetNumberOfDevices();
        std::shared_ptr<devices::DeviceBase> GetDevice(const size_t index);

        size_t SetColor(const LFX_COLOR& color);
        size_t SetColorForLocation(const LightLocationMask locationMask, const LFX_COLOR& color);

        size_t MorphTo(const LFX_COLOR& color, unsigned int time);
        size_t MorphToForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, unsigned int time);

        size_t Pulse(const LFX_COLOR& color, unsigned int time, unsigned int amount);
        size_t PulseForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, unsigned int time, unsigned int amount);
    
    private:
        LightsManager() {};
        LightsManager(const LightsManager&);
        void operator =(const LightsManager&);

        std::vector<std::shared_ptr<devices::DeviceBase>> devices;
        bool isInitialized = false;

        Config config;
        TrayIcon trayIcon;

        void AddDevices();

    };

}

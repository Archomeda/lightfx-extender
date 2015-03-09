#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceManager.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/LogManager.h"
#include "../Config/MainConfigFile.h"
#include "../Devices/DeviceLightFX.h"
#include "../Devices/DeviceLightpack.h"
#include "../Devices/DeviceLogitech.h"
#include "../Devices/LightFX2.h"
#include "../Utils/FileIO.h"


#define LOG(logLevel, line) if (this->GetLightFXExtender() != nullptr) { this->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"DeviceManager - ") + line); }

using namespace std;
using namespace lightfx::config;
using namespace lightfx::devices;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API size_t DeviceManager::InitializeDevices() {
            LOG(LogLevel::Debug, L"Initializing devices");
            size_t i = 0;

            auto config = this->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
            auto lightpack = make_shared<DeviceLightpack>(config->LightpackHost, config->LightpackPort, config->LightpackKey);
            this->AddChild(L"Lightpack", lightpack);
            if (lightpack->Initialize()) {
                ++i;
            }

            auto logitech = make_shared<DeviceLogitech>();
            this->AddChild(L"Logitech", logitech);
            if (logitech->Initialize()) {
                ++i;
            }

            // Load native LightFX devices
            if (InitializeLightFX()) {
                LOG(LogLevel::Debug, L"Alienware LightFX.dll loaded");

                unsigned int numDevices = 0;
                if (LightFX_GetNumDevices(&numDevices) == LFX_SUCCESS) {
                    LOG(LogLevel::Debug, to_wstring(numDevices) + L" LightFX devices found");

                    for (unsigned int j = 0; j < numDevices; ++j) {
                        auto lightFX = make_shared<DeviceLightFX>();
                        bool initialized = lightFX->Initialize(); // We have to initialize it first in order to get the name
                        wstring deviceName = lightFX->GetDeviceName();
                        if (deviceName == L"") {
                            deviceName = L"LightFX " + to_wstring(j);
                        }
                        this->AddChild(lightFX->GetDeviceName(), lightFX);
                        if (initialized) {
                            ++i;
                        }
                    }

                    // TODO: Periodically check for changes (e.g. when a device gets connected or disconnected)
                } else {
                    LOG(LogLevel::Debug, L"Failed to check the number of LightFX devices");
                }
            } else {
                LOG(LogLevel::Debug, L"Alienware LightFX.dll not found");
            }

            LOG(LogLevel::Info, L"Successfully initialized " + to_wstring(i) + L" devices");

            // Enable devices where needed
            for (pair<wstring, bool> device : config->EnabledDevices) {
                if (device.second) {
                    auto dev = this->GetChild(device.first);
                    if (dev->IsInitialized()) {
                        dev->Enable();
                    }
                }
            }

            return i;
        }

        LFXE_API size_t DeviceManager::UninitializeDevices() {
            LOG(LogLevel::Debug, L"Uninitializing devices");
            size_t i = 0;

            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                if (this->GetChildByIndex(j)->Release()) {
                    ++i;
                }
            }

            // Unload native LightFX devices if needed
            if (IsLightFXInitialized()) {
                if (ReleaseLightFX()) {
                    LOG(LogLevel::Debug, L"Alienware LightFX.dll unloaded");
                } else {
                    LOG(LogLevel::Error, L"Alienware LightFX.dll not unloaded");
                }
            }

            LOG(LogLevel::Info, L"Successfully uninitialized " + to_wstring(i) + L" devices");
            return i;
        }

    }
}

#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceManager.h"

// Project includes
#include "../LightFXExtender.h"
#include "ConfigManager.h"
#include "LogManager.h"
#include "../Config/MainConfigFile.h"
#include "../Devices/DeviceLightFX.h"
#include "../Devices/DeviceLightpack.h"
#include "../Devices/DeviceLogitech.h"
#include "../Devices/LightFX2.h"
#include "../Utils/FileIO.h"


#define LOG(logLevel, message) if (this->GetLightFXExtender() != nullptr) { LOG_(this->GetLightFXExtender()->GetLogManager(), logLevel, wstring(L"DeviceManager - ") + message) }

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
            logitech->SetRange(config->LogitechColorRangeOutMin, config->LogitechColorRangeOutMax, config->LogitechColorRangeInMin, config->LogitechColorRangeInMax);
            this->AddChild(L"Logitech", logitech);
            logitech->SetG110WorkaroundEnabled(config->LogitechG110WorkaroundEnabled);
            if (logitech->Initialize()) {
                ++i;
            }

            // Load native LightFX devices
            if (InitializeLightFX(config->AlienwareDllName, config->AlienwareBackupDllName)) {
                LOG(LogLevel::Debug, L"Alienware LightFX.dll loaded");
                LFX_RESULT result;

                result = LightFX_Initialize();
                if (result == LFX_SUCCESS) {
                    unsigned int numDevices = 0;
                    result = LightFX_GetNumDevices(&numDevices);
                    if (result == LFX_SUCCESS) {
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
                        LOG(LogLevel::Error, L"Failed to check the number of LightFX devices: " + to_wstring(result));
                    }
                } else {
                    LOG(LogLevel::Error, L"Failed to initialize LightFX: " + to_wstring(result));
                }
            } else {
                LOG(LogLevel::Debug, L"Alienware LightFX.dll not found");
            }

            LOG(LogLevel::Info, L"Successfully initialized " + to_wstring(i) + L" devices");

            // Enable devices where needed
            for (pair<wstring, bool> device : config->EnabledDevices) {
                if (device.second) {
                    auto dev = this->GetChild(device.first);
                    if (dev == nullptr) {
                        LOG(LogLevel::Warning, L"Device " + device.first + L" is configured in settings, but was not found in the system");
                        continue;
                    }
                    if (!dev->IsInitialized()) {
                        LOG(LogLevel::Warning, L"Device " + device.first + L" cannot be enabled, because was not initialized");
                        continue;
                    }
                    dev->Enable();
                }
            }

            // Start update timer
            this->StartUpdateTimer();

            return i;
        }

        LFXE_API size_t DeviceManager::UninitializeDevices() {
            LOG(LogLevel::Debug, L"Uninitializing devices");
            size_t i = 0;

            // Stop update timer
            this->StopUpdateTimer();

            // Unload devices
            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                auto device = this->GetChildByIndex(j);
                if (device->IsEnabled() && device->Release()) {
                    ++i;
                }
            }

            // Unload native LightFX devices if needed
            if (IsLightFXInitialized()) {
                LFX_RESULT result = LightFX_Release();
                if (result == LFX_SUCCESS) {
                    if (ReleaseLightFX()) {
                        LOG(LogLevel::Debug, L"Alienware LightFX.dll unloaded");
                    } else {
                        LOG(LogLevel::Error, L"Alienware LightFX.dll not unloaded");
                    }
                } else {
                    LOG(LogLevel::Error, L"Failed to release LightFX: " + to_wstring(result));
                }
            }

            LOG(LogLevel::Info, L"Successfully uninitialized " + to_wstring(i) + L" devices");
            return i;
        }


        LFXE_API void DeviceManager::StartUpdateTimer() {
            if (this->DeviceUpdateTimer == nullptr || !this->DeviceUpdateTimer->IsActive()) {
                this->DeviceUpdateTimer = make_unique<Timer>(10, &DeviceManager::DeviceUpdateTask, this);
            }
        }

        LFXE_API void DeviceManager::StopUpdateTimer() {
            if (this->DeviceUpdateTimer != nullptr && this->DeviceUpdateTimer->IsActive()) {
                this->DeviceUpdateTimer->Stop();
            }
        }

        LFXE_API void DeviceManager::DeviceUpdateTask() {
            for (size_t i = 0; i < this->GetChildrenCount(); ++i) {
                auto device = this->GetChildByIndex(i);
                if (device->IsEnabled()) {
                    device->NotifyUpdate();
                }
            }
        }

    }
}

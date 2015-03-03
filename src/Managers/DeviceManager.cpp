#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceManager.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/LogManager.h"
#include "../Config/MainConfigFile.h"
#include "../Devices/DeviceLightpack.h"
#include "../Devices/DeviceLogitech.h"


#define LOG(logLevel, line) if (this->GetLightFXExtender() != nullptr) { this->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"DeviceManager - ") + line); }

using namespace std;
using namespace lightfx::config;
using namespace lightfx::devices;

namespace lightfx {
    namespace managers {

        size_t DeviceManager::InitializeDevices() {
            LOG(LogLevel::Info, L"Initializing devices");
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

            LOG(LogLevel::Info, L"Successfully initialized " + to_wstring(i) + L" devices");

            // Enable devices where needed
            for (pair<wstring, bool> device : config->EnabledDevices) {
                if (device.first == L"Lightpack" && device.second) {
                    lightpack->Enable();
                } else if (device.first == L"Logitech" && device.second) {
                    logitech->Enable();
                }
            }

            return i;
        }

        size_t DeviceManager::UninitializeDevices() {
            LOG(LogLevel::Info, L"Uninitializing devices");
            size_t i = 0;

            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                if (this->GetChildByIndex(j)->Release()) {
                    ++i;
                }
            }

            LOG(LogLevel::Info, L"Successfully uninitialized " + to_wstring(i) + L" devices");
            return i;
        }

    }
}

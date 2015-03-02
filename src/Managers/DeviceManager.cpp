#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceManager.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/ConfigManager.h"
#include "../Config/MainConfigFile.h"
#include "../Devices/DeviceLightpack.h"
#include "../Devices/DeviceLogitech.h"


using namespace std;
using namespace lightfx::config;
using namespace lightfx::devices;

namespace lightfx {
    namespace managers {

        size_t DeviceManager::InitializeDevices() {
            size_t i = 0;

            auto config = this->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
            auto lightpack = make_shared<DeviceLightpack>(config->LightpackHost, config->LightpackPort, config->LightpackKey);
            if (lightpack->Initialize()) {
                ++i;
                this->AddChild(L"Lightpack", lightpack);
            }

            auto logitech = make_shared<DeviceLogitech>();
            if (logitech->Initialize()) {
                ++i;
                this->AddChild(L"Logitech", logitech);
            }

            return i;
        }

        size_t DeviceManager::UninitializeDevices() {
            size_t i = 0;

            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                if (this->GetChildByIndex(j)->Release()) {
                    ++i;
                }
            }

            return i;
        }

    }
}

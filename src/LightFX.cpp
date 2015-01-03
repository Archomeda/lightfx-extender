#include <map>
#include <string>
#include "LightFX.h"

#include "DeviceLogitech.h"
#include "DeviceMmf.h"

using namespace std;

#define LFX_VERSION "2.2.0.0"

namespace lightfx {

    LightFX::~LightFX() {
        this->Release();
    }

    LFX_RESULT LightFX::Initialize() {
        if (!this->isInitialized) {
            // Load config
            this->config.Load();
            this->config.Save();

            if (this->config.LogitechEnabled) {
                // Add Logitech device
                auto devLogi = unique_ptr<DeviceLogitech>(new DeviceLogitech());
                devLogi->Initialize();
                this->devices.push_back(move(devLogi));
            }

            if (this->config.MemoryMappedFileEnabled) {
                // Add Memory Mapped File devices
                for (size_t i = 0; i < this->config.MemoryMappedFileDevices.size(); ++i) {
                    auto devMmf = unique_ptr<DeviceMmf>(new DeviceMmf(this->config.MemoryMappedFileDevices[i].Description,
                        this->config.MemoryMappedFileDevices[i].Type, this->config.MemoryMappedFileDevices[i].Lights));
                    devMmf->Initialize();
                    this->devices.push_back(move(devMmf));
                }
            }

            this->isInitialized = true;
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::Release() {
        if (this->isInitialized) {
            for (auto& device : this->devices) {
                device->Release();
            }

            this->isInitialized = false;
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::Reset() {
        for (auto& device : this->devices) {
            auto result = device->Reset();
            if (result != LFX_SUCCESS) {
                return result;
            }
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::Update() {
        for (auto& device : this->devices) {
            auto result = device->Update();
            if (result != LFX_SUCCESS) {
                return result;
            }
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::UpdateDefault() {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        return LFX_FAILURE; // Not currently supported
    }

    LFX_RESULT LightFX::GetNumDevices(unsigned int& numDevices) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        numDevices = static_cast<unsigned int>(devices.size());
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetDeviceDescription(const unsigned int devIndex, string& devDesc, const unsigned int devDescSize, unsigned char& devType) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        auto result = this->devices[devIndex]->GetDeviceInfo(devDesc, devType);
        if (devDescSize < devDesc.length()) {
            return LFX_ERROR_BUFFSIZE;
        }

        return result;
    }

    LFX_RESULT LightFX::GetNumLights(const unsigned int devIndex, unsigned int& numLights) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        return this->devices[devIndex]->GetNumLights(numLights);
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, string& lightDesc, const unsigned int devDescSize) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        unsigned int numLights;
        this->GetNumLights(devIndex, numLights);
        if (lightIndex > numLights) {
            return LFX_ERROR_NOLIGHTS;
        }

        auto result = this->devices[devIndex]->GetLightDescription(lightIndex, lightDesc);
        if (devDescSize < lightDesc.length()) {
            return LFX_ERROR_BUFFSIZE;
        }

        return result;
    }

    LFX_RESULT LightFX::GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, LFX_POSITION& lightLoc) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        unsigned int numLights;
        this->GetNumLights(devIndex, numLights);
        if (lightIndex > numLights) {
            return LFX_ERROR_NOLIGHTS;
        }

        return this->devices[devIndex]->GetLightLocation(lightIndex, lightLoc);
    }

    LFX_RESULT LightFX::GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, LFX_COLOR& lightCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        unsigned int numLights;
        this->GetNumLights(devIndex, numLights);
        if (lightIndex > numLights) {
            return LFX_ERROR_NOLIGHTS;
        }

        return this->devices[devIndex]->GetLightColor(lightIndex, lightCol);
    }

    LFX_RESULT LightFX::SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const LFX_COLOR& lightCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        unsigned int numLights;
        this->GetNumLights(devIndex, numLights);
        if (lightIndex > numLights) {
            return LFX_ERROR_NOLIGHTS;
        }

        return this->devices[devIndex]->SetLightColor(lightIndex, lightCol);
    }

    LFX_RESULT LightFX::Light(const unsigned int locationMask, const LFX_COLOR& lightCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        for (auto& device : this->devices) {
            auto result = device->Light(locationMask, lightCol);
            if (result != LFX_SUCCESS) {
                return result;
            }
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const LFX_COLOR& primaryCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        unsigned int numLights;
        this->GetNumLights(devIndex, numLights);
        if (lightIndex > numLights) {
            return LFX_ERROR_NOLIGHTS;
        }

        return this->devices[devIndex]->SetLightActionColor(lightIndex, actionType, primaryCol);
    }

    LFX_RESULT LightFX::SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > devices.size()) {
            return LFX_ERROR_NODEVS;
        }

        unsigned int numLights;
        this->GetNumLights(devIndex, numLights);
        if (lightIndex > numLights) {
            return LFX_ERROR_NOLIGHTS;
        }

        return this->devices[devIndex]->SetLightActionColor(lightIndex, actionType, primaryCol, secondaryCol);
    }

    LFX_RESULT LightFX::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        for (auto& device : this->devices) {
            auto result = device->ActionColor(locationMask, actionType, primaryCol);
            if (result != LFX_SUCCESS) {
                return result;
            }
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        for (auto& device : this->devices) {
            auto result = device->ActionColor(locationMask, actionType, primaryCol, secondaryCol);
            if (result != LFX_SUCCESS) {
                return result;
            }
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::SetTiming(const int newTiming) {
        for (auto& device : this->devices) {
            auto result = device->SetTiming(newTiming);
            if (result != LFX_SUCCESS) {
                return result;
            }
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetVersion(string& version, const unsigned int versionSize) {
        string ver = LFX_VERSION;
        if (versionSize < ver.length()) {
            return LFX_ERROR_BUFFSIZE;
        }

        version = ver;
        return LFX_SUCCESS;
    }

}

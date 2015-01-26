#pragma once

// Standard includes
#include <string>
#include <vector>

// 3rd party includes
#include "LFXDecl.h"

// Project includes
#include "../LightLocationMask.h"
#include "DeviceLight.h"


namespace lightfx {
    namespace devices {

        class DeviceBase {

        public:
            DeviceBase();

            bool IsEnabled() { return this->IsEnabled_; }
            bool IsInitialized() { return this->IsInitialized_; }

            virtual bool EnableDevice();
            virtual bool DisableDevice();

            virtual bool Initialize();
            virtual bool Release();
            virtual bool Update();
            virtual bool Reset();

            virtual std::wstring GetDeviceName() = 0;
            virtual unsigned char GetDeviceType() = 0;

            virtual size_t GetNumberOfLights();
            virtual DeviceLight GetLight(const size_t index);

            virtual LFX_COLOR GetPrimaryColorForLight(const size_t index);
            virtual bool SetPrimaryColor(const LFX_COLOR& color);
            virtual bool SetPrimaryColorForLight(const size_t index, const LFX_COLOR& color);
            virtual bool SetPrimaryColorForLocation(const LightLocationMask locationMask, const LFX_COLOR& color);

        protected:
            bool IsEnabled_ = false;
            bool IsInitialized_ = false;

            std::vector<DeviceLight> Lights = {};
            std::vector<LFX_COLOR> CurrentPrimaryColor = {};
            std::vector<LFX_COLOR> NextPrimaryColor = {};

        };

    }
}

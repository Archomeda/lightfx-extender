#pragma once

// Standard includes
#include <string>
#include <memory>

// Project includes
#include "LightManager.h"

// API exports
#include "../Common/ApiExports.h"



namespace lightfx {
    namespace devices {

        class LightManager;
        typedef std::shared_ptr<LightManager> spLightManager;

        class LFXE_API Device {

        public:
            Device();
            ~Device();

            bool IsEnabled();
            bool IsInitialized();

            virtual bool Enable();
            virtual bool Disable();

            virtual bool Initialize();
            virtual bool Release();
            virtual bool Update();
            virtual bool Reset();

            virtual std::wstring GetDeviceName() = 0;
            virtual unsigned char GetDeviceType() = 0;

        private:
            bool isEnabled = false;
            bool isInitialized = false;
            spLightManager lightManager;

        };

    }
}

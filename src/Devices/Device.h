#pragma once

// Standard includes
#include <string>

// Project includes
#include "../Managers/DeviceManager.h"
#include "../Managers/ChildOfManager.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {
        class DeviceManager;
    }

    namespace devices {

        class LFXE_API Device : public managers::ChildOfManager < managers::DeviceManager > {

        public:
            Device();

            bool IsEnabled();
            bool IsInitialized();

            virtual bool Enable();
            virtual bool Disable();

            virtual bool Initialize();
            virtual bool Release();
            virtual bool Update();
            virtual bool Reset();

            virtual const std::wstring GetDeviceName() = 0;
            virtual const unsigned char GetDeviceType() = 0;


        private:
            bool isEnabled = false;
            bool isInitialized = false;

        };

    }
}

#pragma warning(pop)

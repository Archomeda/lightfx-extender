#pragma once

// Windows includes
#include "Device.h"

// Project includes
#include "Proxies/LightFX2Proxy.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace devices {

        class LFXE_API DeviceLightFX : public Device {

        public:
            virtual bool Initialize() override;
            virtual bool Release() override;

            virtual const unsigned int GetDeviceIndex();
            virtual void SetDeviceIndex(const unsigned int deviceIndex);
            virtual const std::wstring GetDeviceName() override;
            virtual void SetDeviceName(const std::wstring& deviceName);
            virtual const DeviceType GetDeviceType() override;
            virtual void SetDeviceType(const DeviceType deviceType);

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

        private:
            std::unique_ptr<proxies::LightFX2Proxy> library;

            unsigned int deviceIndex = 0;
            std::wstring deviceName = L"Unknown LightFX device";
            DeviceType deviceType = DeviceType::DeviceUnknown;

        };

    }
}

#pragma warning(pop)

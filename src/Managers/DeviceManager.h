#pragma once

// Standard includes
#include <string>
#include <map>
#include <memory>

// Project includes
#include "../LightFXExtender.h"
#include "../Devices/Device.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    class LightFXExtender;

    namespace managers {

        typedef std::shared_ptr<devices::Device> spDevice;
        typedef std::map<std::wstring, spDevice> mapDevices;

        class LFXE_API DeviceManager {

        public:
            DeviceManager(const LightFXExtender* lightFXExtender);
            ~DeviceManager();

            void AddDevice(const std::wstring& deviceKey, const spDevice& device);
            spDevice GetDevice(const std::wstring& deviceKey);

#pragma warning(push)
#pragma warning(disable : 4251)

        private:
            mapDevices devices = {};

#pragma warning(pop)

        };

    }
}

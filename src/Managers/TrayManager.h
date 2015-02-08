#pragma once

// Standard includes
#include <string>
#include <vector>

// Project includes
#include "../LightFXExtender.h"
#include "DeviceManager.h"
#include "../Devices/Device.h"


// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    class LightFXExtender;

    namespace managers {

        // Re-typedef because for some reason the compiler cannot always find it in DeviceManager.h
        typedef std::shared_ptr<devices::Device> spDevice;

        class LFXE_API TrayManager {

        public:
            TrayManager(const LightFXExtender* lightFXExtender);
            ~TrayManager();

            bool IsAdded();
            void AddTrayIcon();
            void RemoveTrayIcon();

            void AttachDevice(const spDevice& device);
            void RemoveDevice(const spDevice& device);

            bool HasUpdateNotification();
            void SetUpdateNotification(const std::wstring& versionString, const std::wstring& downloadUrl);

        };

    }
}

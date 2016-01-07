#pragma once

// Standard includes
#include <atomic>
#include <memory>
#include <thread>

// Project includes
#include "ManagerWithChildren.h"
#include "../Vendors/VendorDevice.h"
#include "../Vendors/VendorLightpack.h"
#include "../Vendors/VendorLogitech.h"
#include "../Utils/NotifyEvent.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {

        class LFXE_API VendorManager : public ManagerWithChildren<vendors::VendorDevice, VendorManager> {

        public:
            VendorManager() {}
            ~VendorManager() {
                this->StopUpdateWorker();
                this->ReleaseHardware();
            }

            size_t InitializeHardware();
            size_t ReleaseHardware();

            std::vector<std::shared_ptr<vendors::VendorBase>> GetVendors() const;

            void StartUpdateWorker();
            void StopUpdateWorker();
            void UpdateLights();

        protected:
            size_t EnableVendorDevices(const std::shared_ptr<vendors::VendorBase>& vendor);
            void UpdateWorker();

        private:
            // Vendors
            std::vector<std::shared_ptr<vendors::VendorBase>> vendors;
            std::shared_ptr<vendors::VendorLogitech> vendorLogitech;
            std::shared_ptr<vendors::VendorLightpack> vendorLightpack;

            bool isInitialized = false;

            bool updateWorkerActive = false;
            std::atomic<bool> stopUpdateWorker = false;
            std::thread updateWorkerThread;
            utils::NotifyEvent updateWorkerNotifyEvent;

            int updateWorkerInterval = 20;

        };

    }
}

#pragma warning(pop)

#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "VendorManager.h"

// Project includes
#include "../LightFXExtender.h"
#include "ConfigManager.h"
#include "../Config/MainConfigFile.h"
#include "../Vendors/VendorLogitech.h"
#include "../Utils/FileIO.h"
#include "../Utils/Log.h"
#include "../Utils/String.h"


using namespace std;
using namespace lightfx::config;
using namespace lightfx::vendors;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API size_t VendorManager::InitializeHardware() {
            if (!this->isInitialized) {
                LOG_INFO(L"Initializing hardware...");
                size_t count = 0;

                // Set some configurations
                auto config = this->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
                this->updateWorkerInterval = config->TimelineUpdateInterval;

                // Initialize Alienware
                this->vendorAlienware = make_shared<VendorAlienware>();
                this->vendorAlienware->SetBackupFilename(config->AlienwareBackupDllName);
                if (this->vendorAlienware->IsLibraryAvailable()) {
                    this->vendorAlienware->InitializeDevices();
                    count += this->EnableVendorDevices(this->vendorAlienware);
                    this->vendors.push_back(this->vendorAlienware);
                } else {
                    LOG_INFO(L"Alienware library not found; if you do have Alienware devices, make sure you correctly set the backup filename of the Alienware DLL file if the game provides a LightFX.dll of its own, and make sure that Alienware Command Center is installed correctly");
                }

                // Initialize Corsair
                this->vendorCorsair = make_shared<VendorCorsair>();
                if (this->vendorCorsair->IsLibraryAvailable()) {
                    this->vendorCorsair->InitializeDevices();
                    count += this->EnableVendorDevices(this->vendorCorsair);
                    this->vendors.push_back(this->vendorCorsair);
                } else {
                    LOG_INFO(L"Corsair library not found; if you do have Corsair devices, make sure you installed Corsair Utility Engine 1.10 or higher and have enabled SDK access in the settings");
                }

                // Initialize Lightpack
                this->vendorLightpack = make_shared<VendorLightpack>();
                this->vendorLightpack->SetHostname(config->LightpackHost);
                this->vendorLightpack->SetPort(config->LightpackPort);
                this->vendorLightpack->SetKey(config->LightpackKey);
                if (this->vendorLightpack->IsLibraryAvailable()) {
                    this->vendorLightpack->InitializeDevices();
                    count += this->EnableVendorDevices(this->vendorLightpack);
                    this->vendors.push_back(this->vendorLightpack);
                } else {
                    LOG_INFO(L"Lightpack library not found; if you do have a Lightpack device, make sure you have installed Prismatik 5.11.1 or higher and have enabled the API socket in the settings");
                }

                // Initialize Logitech
                this->vendorLogitech = make_shared<VendorLogitech>();
                this->vendorLogitech->SetRange(config->LogitechColorRangeOutMin, config->LogitechColorRangeOutMax, config->LogitechColorRangeInMin, config->LogitechColorRangeInMax);
                this->vendorLogitech->SetRestoreLightsWhenOffEnabled(config->LogitechRestoreLightsOnNullEnabled);
                this->vendorLogitech->SetG110WorkaroundEnabled(config->LogitechG110WorkaroundEnabled);
                if (this->vendorLogitech->IsLibraryAvailable()) {
                    this->vendorLogitech->InitializeDevices();
                    count += this->EnableVendorDevices(this->vendorLogitech);
                    this->vendors.push_back(this->vendorLogitech);
                } else {
                    LOG_INFO(L"Logitech library not found; if you do have Logitech devices, make sure you have installed Logitech Gaming Software 8.70.315 or higher and have allowed third party access to the illumination SDK in the settings");
                }

                this->isInitialized = true;
                return count;
            }
            return this->GetChildrenCount();
        }

        LFXE_API size_t VendorManager::ReleaseHardware() {
            if (this->isInitialized) {
                LOG_INFO(L"Releasing hardware...");
                size_t count = 0;

                for (size_t i = 0; i < this->GetChildrenCount(); ++i) {
                    auto device = this->GetChildByIndex(i);
                    wstring deviceName = device->GetVendor()->GetVendorName() + L" - " + device->GetDeviceName();
                    if (device->IsEnabled()) {
                        LOG_INFO(L"Disabling hardware " + deviceName + L"...");
                        if (device->Disable()) {
                            LOG_INFO(L"Disabled hardware " + deviceName);
                            ++count;
                        } else {
                            LOG_WARNING(L"Could not disable hardware " + deviceName);
                        }
                    }
                }

                this->isInitialized = false;
                return count;
            }
            return 0;
        }

        LFXE_API vector<shared_ptr<VendorBase>> VendorManager::GetVendors() const {
            return this->vendors;
        }

        LFXE_API void VendorManager::StartUpdateWorker() {
            if (!this->updateWorkerActive) {
                this->updateWorkerActive = true;
                this->stopUpdateWorker = false;
                this->updateWorkerThread = thread(&VendorManager::UpdateWorker, this);
            }
        }

        LFXE_API void VendorManager::StopUpdateWorker() {
            if (this->updateWorkerActive) {
                this->stopUpdateWorker = true;
                this->updateWorkerNotifyEvent.Notify();
                if (this->updateWorkerThread.joinable()) {
                    this->updateWorkerThread.join();
                }
                this->updateWorkerActive = false;
            }
        }

        LFXE_API void VendorManager::UpdateLights() {
            if (this->updateWorkerActive) {
                this->updateWorkerNotifyEvent.Notify();
            }
        }

        LFXE_API size_t VendorManager::EnableVendorDevices(const shared_ptr<VendorBase>& vendor) {
            size_t count = 0;
            
            if (vendor->IsLibraryAvailable()) {
                auto config = this->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
                for (auto pair : vendor->GetDevices()) {
                    auto device = pair.second;
                    wstring deviceNameConfig = vendor->GetVendorName() + L" - " + device->GetDeviceName();
                    auto it = config->EnabledDevices.find(deviceNameConfig);
                    if (it != config->EnabledDevices.end() && config->EnabledDevices[deviceNameConfig]) {
                        // This device is enabled in the settings
                        LOG_INFO(L"Enabling hardware " + deviceNameConfig + L"...");
                        if (device->Enable()) {
                            LOG_INFO(L"Enabled hardware " + deviceNameConfig);
                            this->AddChild(deviceNameConfig, device);
                        } else {
                            LOG_WARNING(L"Could not enable hardware " + deviceNameConfig);
                        }
                    }
                }
            }

            return count;
        }

        LFXE_API void VendorManager::UpdateWorker() {
            bool hasTimeline = false;

            while (!this->stopUpdateWorker) {
                if (hasTimeline && !this->updateWorkerNotifyEvent.IsNotified()) {
                    // Continue with the current timeline(s), but wait for a certain amount of time before doing so
                    this_thread::sleep_for(chrono::milliseconds(this->updateWorkerInterval));
                } else {
                    // No timeline active, let's halt and wait until we get signalled
                    this->updateWorkerNotifyEvent.Wait();

                    // Reset variables
                    hasTimeline = false;
                }

                // Stop if we were signalled to stop
                if (this->stopUpdateWorker) {
                    break;
                }

                // Update devices
                bool stillHasTimeline = false;
                for (size_t i = 0; i < this->GetChildrenCount(); ++i) {
                    auto device = this->GetChildByIndex(i);
                    if (!hasTimeline) {
                        device->StartQueuedTimeline();
                    }
                    chrono::milliseconds timeTick = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
                    switch (device->Update(timeTick)) {
                    case VendorDeviceUpdateStatus::TimelineStarted:
                    case VendorDeviceUpdateStatus::TimelineBusy:
                        stillHasTimeline = true;
                        break;
                    }
                }
                hasTimeline = stillHasTimeline;
            }
        }

    }
}

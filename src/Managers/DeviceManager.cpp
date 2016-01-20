#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceManager.h"

// Project includes
#include "../LightFXExtender.h"
#include "ConfigManager.h"
#include "../Config/MainConfigFile.h"
#include "../Devices/DeviceLightFX.h"
#include "../Devices/DeviceLightpack.h"
#include "../Devices/DeviceLogitech.h"
#include "../Devices/DeviceCorsair.h"
#include "../Devices/DeviceRazer.h"
#include "../Utils/FileIO.h"
#include "../Utils/Log.h"
#include "../Utils/String.h"


using namespace std;
using namespace lightfx::config;
using namespace lightfx::devices;
using namespace lightfx::devices::proxies;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API size_t DeviceManager::InitializeDevices() {
            LOG_DEBUG(L"Initializing devices");
            size_t i = 0;

            auto config = this->GetLightFXExtender()->GetConfigManager()->GetMainConfig();
            this->updateDevicesInterval = config->TimelineUpdateInterval;

            auto lightpack = make_shared<DeviceLightpack>(config->LightpackHost, config->LightpackPort, config->LightpackKey);
            this->AddChild(L"Lightpack", lightpack);
            if (lightpack->Initialize()) {
                ++i;
            }


            auto logitech = make_shared<DeviceLogitech>();
            logitech->SetRange(config->LogitechColorRangeOutMin, config->LogitechColorRangeOutMax, config->LogitechColorRangeInMin, config->LogitechColorRangeInMax);
            this->AddChild(L"Logitech", logitech);
            logitech->SetRestoreLightsOnNullEnabled(config->LogitechRestoreLightsOnNullEnabled);
            logitech->SetG110WorkaroundEnabled(config->LogitechG110WorkaroundEnabled);
            if (logitech->Initialize()) {
                ++i;
            }

            
            auto corsair = make_shared<DeviceCorsair>();
            this->AddChild(L"Corsair", corsair);
            if (corsair->Initialize()) {
                ++i;
            }
            
            auto razer = make_shared<DeviceRazer>();
            razer->SetHardware(config->RazerUseWithKeyboard, config->RazerUseWithMouse, config->RazerUseWithHeadset, config->RazerUseWithMousepad, config->RazerUseWithKeypad);
            this->AddChild(L"Razer", razer);
            if (razer->Initialize()) {
                ++i;
            }

            // Load native LightFX devices
            this->lightFXLibrary = unique_ptr<LightFX2Proxy>(new LightFX2Proxy(config->AlienwareDllName, config->AlienwareBackupDllName));
            if (this->lightFXLibrary->Load()) {
                LOG_DEBUG(L"Alienware LightFX library found");
                LFX_RESULT result;

                result = this->lightFXLibrary->LFX_Initialize();
                if (result == LFX_SUCCESS) {
                    unsigned int numDevices = 0;
                    result = this->lightFXLibrary->LFX_GetNumDevices(&numDevices);
                    if (result == LFX_SUCCESS) {
                        LOG_DEBUG(to_wstring(numDevices) + L" LightFX devices found");

                        for (unsigned int j = 0; j < numDevices; ++j) {
                            auto lightFX = make_shared<DeviceLightFX>();
                            lightFX->SetDeviceIndex(j);

                            // Get device name first so we can properly add it to the list of devices
                            char* devDesc = new char[LFX_MAX_STRING_SIZE];
                            unsigned char devType = 0;
                            if (this->lightFXLibrary->LFX_GetDeviceDescription(j, devDesc, LFX_MAX_STRING_SIZE, &devType) == LFX_SUCCESS) {
                                wstring deviceName = string_to_wstring(devDesc);
                                if (deviceName == L"") {
                                    deviceName = L"LightFX " + to_wstring(j);
                                }
                                this->AddChild(deviceName, lightFX);                                
                                if (lightFX->Initialize()) {
                                    ++i;
                                }
                                lightFX->SetDeviceName(deviceName);
                            } else {
                                LOG_ERROR(L"Failed to get the device name of LightFX device " + to_wstring(j));
                            }
                            LFX_SAFE_DELETE_ARRAY(devDesc);
                        }

                        //TODO: Periodically check for changes (e.g. when a device gets connected or disconnected)
                    } else {
                        LOG_ERROR(L"Failed to check the number of LightFX devices: " + this->lightFXLibrary->LfxResultToString(result));
                    }
                } else {
                    LOG_ERROR(L"Failed to initialize LightFX: " + this->lightFXLibrary->LfxResultToString(result));
                }
            } else {
                LOG_DEBUG(L"Alienware LightFX library not found");
            }

            LOG_INFO(L"Successfully initialized " + to_wstring(i) + L" devices");

            // Enable devices where needed
            if (config->AutoDeviceDetection)
            {
                for (size_t i = 0; i < this->GetChildrenCount(); ++i) {
                    auto device = this->GetChildByIndex(i);

                    if (device == nullptr) {
                        LOG_WARNING(L"Device " + device->GetDeviceName() + L" is configured in settings, but was not found in the system");
                        continue;
                    }
                    if (!device->IsInitialized()) {
                        LOG_WARNING(L"Device " + device->GetDeviceName() + L" cannot be enabled, because was not initialized");
                        continue;
                    }

                    bool auto_result = device->Enable();
                    LOG_WARNING(L"Device " + device->GetDeviceName() + L" was automatically set to " + (auto_result ? L"ON" : L"OFF"));
                }
            }
            else
            {
                for (pair<wstring, bool> device : config->EnabledDevices) {
                    if (device.second) {
                        auto dev = this->GetChild(device.first);
                        if (dev == nullptr) {
                            LOG_WARNING(L"Device " + device.first + L" is configured in settings, but was not found in the system");
                            continue;
                        }
                        if (!dev->IsInitialized()) {
                            LOG_WARNING(L"Device " + device.first + L" cannot be enabled, because was not initialized");
                            continue;
                        }
                        dev->Enable();
                    }
                }
            }
            return i;
        }

        LFXE_API size_t DeviceManager::UninitializeDevices() {
            LOG_DEBUG(L"Uninitializing devices");
            size_t i = 0;

            // Unload devices
            for (size_t j = 0; j < this->GetChildrenCount(); ++j) {
                auto device = this->GetChildByIndex(j);
                if (device->IsEnabled() && device->Release()) {
                    ++i;
                }
            }

            // Unload native LightFX devices if needed
            if (this->lightFXLibrary->IsLoaded()) {
                LFX_RESULT result = this->lightFXLibrary->LFX_Release();
                if (result == LFX_SUCCESS) {
                    if (this->lightFXLibrary->Unload()) {
                        LOG_DEBUG(L"Alienware LightFX library unloaded");
                    } else {
                        LOG_ERROR(L"Could not unload Alienware LightFX library");
                    }
                } else {
                    LOG_ERROR(L"Failed to release LightFX: " + to_wstring(result));
                }
            }

            LOG_INFO(L"Successfully uninitialized " + to_wstring(i) + L" devices");
            return i;
        }
   
        LFXE_API void DeviceManager::StartUpdateDevicesWorker() {
            if (!this->updateDevicesWorkerActive) {
                this->updateDevicesWorkerActive = true;
                this->stopUpdateDevicesWorker = false;
                this->updateDevicesWorkerThread = thread(&DeviceManager::UpdateDevicesWorker, this);
            }
        }

        LFXE_API void DeviceManager::StopUpdateDevicesWorker() {
            if (this->updateDevicesWorkerActive) {
                this->stopUpdateDevicesWorker = true;
                this->updateDevicesNotifyEvent.Notify();
                if (this->updateDevicesWorkerThread.joinable()) {
                    this->updateDevicesWorkerThread.join();
                }
                this->updateDevicesWorkerActive = false;
            }
        }

        LFXE_API void DeviceManager::UpdateDeviceLights(const bool flushQueue) {
            if (this->updateDevicesWorkerActive) {
                this->flushQueue = flushQueue;
                this->updateDevicesNotifyEvent.Notify();
            }
        }

        LFXE_API void DeviceManager::UpdateDevicesWorker() {
            bool isUpdating = false;
            chrono::milliseconds timeTick;

            while (!this->stopUpdateDevicesWorker) {
                try {
                    bool flushQueue = false;
                    if (isUpdating && !this->updateDevicesNotifyEvent.IsNotified()) {
                        // Still updating from previous iterations without new updates coming in, sleep for a while to prevent unneeded CPU usage
                        this_thread::sleep_for(chrono::milliseconds(this->updateDevicesInterval));
                    } else {
                        // Wait for when we get signaled to update or stop
                        this->updateDevicesNotifyEvent.Wait();

                        // Reset certain variables since we should have a new timeline here
                        flushQueue = this->flushQueue;
                        this->flushQueue = false;
                        isUpdating = false;
                    }

                    // Stop worker if it has been signaled to stop
                    if (this->stopUpdateDevicesWorker) {
                        break;
                    }

                    // Update every device
                    bool done = true;
                    timeTick = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
                    for (size_t i = 0; i < this->GetChildrenCount(); ++i) {
                        if (!isUpdating) {
                            // This update is done for the first time, commit queued timeline and optionally flush the queue
                            this->GetChildByIndex(i)->CommitQueuedTimeline(flushQueue);
                        }
                        done &= this->GetChildByIndex(i)->Update(timeTick);
                    }

                    isUpdating = !done;
                } catch (const exception& ex) {
                    LOG_ERROR(L"Caught exception in DeviceManager thread: " + string_to_wstring(ex.what()));
                }
            }
        }

    }
}

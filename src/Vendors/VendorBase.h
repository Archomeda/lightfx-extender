#pragma once

// Standard includes
#include <map>
#include <memory>

// Project includes
#include "../Timelines/LightColor.h"
#include "VendorDevice.h"
#include "Libraries/LibraryBase.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace vendors {

        class LFXE_API VendorBase {

        public:
            VendorBase() {}
            virtual ~VendorBase() {}

            virtual std::wstring GetVendorName() const = 0;
            
            virtual std::shared_ptr<libraries::LibraryBase> GetLibrary() const = 0;
            bool IsLibraryAvailable() {
                return this->GetLibrary()->IsLibraryAvailable();
            }
            bool InitializeLibrary() {
                return this->GetLibrary()->InitializeLibrary();
            }
            bool ReleaseLibrary() {
                return this->GetLibrary()->ReleaseLibrary();
            }

            const std::map<unsigned int, std::shared_ptr<VendorDevice>>& GetDevices() const {
                return this->devices;
            }
            const std::shared_ptr<VendorDevice>& GetDevice(const unsigned int id) const {
                return this->devices.at(id);
            }
            virtual void InitializeDevices() = 0;
            virtual void ReleaseDevices() {
                if (this->GetLibrary()->IsLibraryInitialized()) {
                    // Force disables, since we can't rely on the device destructor to clean itself up
                    for (auto device : this->GetDevices()) {
                        device.second->Disable();
                    }

                    // Clear device mappings
                    this->devices.clear();
                }
            };

            bool IsDeviceEnabled(const unsigned int id) const {
                auto it = this->enabledDevices.find(id);
                if (it != this->enabledDevices.end()) {
                    return it->second;
                }
                return false;
            }
            virtual bool EnableDevice(const unsigned int id) = 0;
            virtual bool DisableDevice(const unsigned int id) = 0;
            virtual bool ResetDevice(const unsigned int id) = 0;

            virtual bool PushColorsToDevice(const unsigned int id, const std::vector<timelines::LightColor>& colors) = 0;

        protected:
            void AddDevice(const unsigned int id, const std::shared_ptr<VendorDevice>& device) {
                this->devices.emplace(id, device);
            }
            void RemoveDevice(const unsigned int id) {
                this->devices.erase(id);
            }

            std::map<unsigned int, std::shared_ptr<VendorDevice>> devices = {};
            std::map<unsigned int, bool> enabledDevices = {};

        };

    }
}

#pragma warning(pop)

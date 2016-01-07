#pragma once

// Standard includes
#include <string>

// Project includes
#include "../../src/Vendors/VendorBase.h"

#include "Libraries/LibraryBaseMock.h"


namespace lightfx_tests {
    namespace vendors {

        class VendorBaseMock : public lightfx::vendors::VendorBase {

        public:
            virtual std::wstring GetVendorName() const override { return L"VendorBaseMock"; }
            virtual std::shared_ptr<lightfx::vendors::libraries::LibraryBase> GetLibrary() const override { return std::make_shared<libraries::LibraryBaseMock>(); }

            using VendorBase::IsLibraryAvailable;
            using VendorBase::InitializeLibrary;
            using VendorBase::ReleaseLibrary;

            using VendorBase::GetDevices;
            using VendorBase::GetDevice;
            virtual void InitializeDevices() override {}
            using VendorBase::ReleaseDevices;

            using VendorBase::IsDeviceEnabled;
            virtual bool EnableDevice(const unsigned int id) override { return true; }
            virtual bool DisableDevice(const unsigned int id) override { return true; }
            virtual bool ResetDevice(const unsigned int id) override { return true; }

            virtual bool PushColorsToDevice(const unsigned int id, const std::vector<lightfx::timelines::LightColor>& colors) override { return true; }

        };

    }
}

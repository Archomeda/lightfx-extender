#pragma once

// Standard includes
#include <string>

// Project includes
#include "VendorBase.h"
#include "Libraries/LibraryCorsair.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace vendors {

        class LFXE_API VendorCorsair : public VendorBase, public std::enable_shared_from_this<VendorCorsair> {

        public:
            VendorCorsair() {}
            virtual ~VendorCorsair() {
                this->ReleaseDevices();
                this->ReleaseLibrary();
            }

            virtual std::wstring GetVendorName() const override { return L"Corsair"; }

            virtual std::shared_ptr<libraries::LibraryBase> GetLibrary() const override { return this->library; }

            virtual void InitializeDevices() override;

            virtual bool EnableDevice(const unsigned int id) override;
            virtual bool DisableDevice(const unsigned int id) override;
            virtual bool ResetDevice(const unsigned int id) override;

        protected:
            virtual bool PushColorsToDevice(const unsigned int id, const std::vector<timelines::LightColor>& colors) override;

            VendorDeviceType CorsairDeviceTypeToVendorDeviceType(const CorsairDeviceType deviceType) const;
        
        private:
            std::shared_ptr<libraries::LibraryCorsair> library = std::make_shared<libraries::LibraryCorsair>();
        };

    }
}

#pragma warning(pop)

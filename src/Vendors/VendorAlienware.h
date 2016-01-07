#pragma once

// Project includes
#include "VendorBase.h"
#include "Libraries/LibraryAlienware.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace vendors {

        class LFXE_API VendorAlienware : public VendorBase, public std::enable_shared_from_this<VendorAlienware> {

        public:
            VendorAlienware() {}
            virtual ~VendorAlienware() {
                this->ReleaseDevices();
                this->ReleaseLibrary();
            }

            virtual std::wstring GetVendorName() const override { return L"Alienware"; }

            virtual std::shared_ptr<libraries::LibraryBase> GetLibrary() const override { return this->library; }

            virtual void InitializeDevices() override;

            virtual bool EnableDevice(const unsigned int id) override;
            virtual bool DisableDevice(const unsigned int id) override;
            virtual bool ResetDevice(const unsigned int id) override;

            // Custom settings
            void SetBackupFilename(const std::wstring& filename) { this->library->SetBackupFilename(filename); }

        protected:
            virtual bool PushColorsToDevice(const unsigned int id, const std::vector<timelines::LightColor>& colors) override;

            VendorDeviceType AlienwareDeviceTypeToVendorDeviceType(const libraries::AlienwareDeviceType deviceType) const;
            VendorDeviceLightPosition AlienwareLightPositionToVendorDeviceLightPosition(const LFX_POSITION& lightPosition) const;
            LFX_COLOR TimelineLightColorToAlienwareLightColor(const timelines::LightColor& lightColor) const;

        private:
            std::shared_ptr<libraries::LibraryAlienware> library = std::make_shared<libraries::LibraryAlienware>();
        };

    }
}

#pragma warning(pop)

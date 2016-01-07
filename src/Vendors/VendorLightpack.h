#pragma once

// Project includes
#include "VendorBase.h"
#include "Libraries/LibraryLightpack.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace vendors {

        class LFXE_API VendorLightpack : public VendorBase, public std::enable_shared_from_this<VendorLightpack> {

        public:
            VendorLightpack() {}
            virtual ~VendorLightpack() {
                this->ReleaseDevices();
                this->ReleaseLibrary();
            }

            virtual std::wstring GetVendorName() const override { return L"Lightpack"; }

            virtual std::shared_ptr<libraries::LibraryBase> GetLibrary() const override { return this->library; }

            virtual void InitializeDevices() override;

            virtual bool EnableDevice(const unsigned int id) override;
            virtual bool DisableDevice(const unsigned int id) override;
            virtual bool ResetDevice(const unsigned int id) override;

            // Custom settings
            void SetHostname(const std::wstring& hostname) { this->library->SetHostname(hostname); }
            void SetPort(const std::wstring& port) { this->library->SetPort(port); }
            void SetKey(const std::wstring& key) { this->key = key; }

        protected:
            virtual bool PushColorsToDevice(const unsigned int id, const std::vector<timelines::LightColor>& colors) override;

        private:
            std::shared_ptr<libraries::LibraryLightpack> library = std::make_shared<libraries::LibraryLightpack>();

            // Custom settings
            std::wstring key = L"";

        };

    }
}

#pragma warning(pop)

#pragma once

// Project includes
#include "VendorBase.h"
#include "Libraries/LibraryLogitech.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace vendors {

        class LFXE_API VendorLogitech : public VendorBase, public std::enable_shared_from_this<VendorLogitech> {

        public:
            VendorLogitech() {}
            virtual ~VendorLogitech() {
                this->ReleaseDevices();
                this->ReleaseLibrary();
            }

            virtual std::wstring GetVendorName() const override { return L"Logitech"; }

            virtual std::shared_ptr<libraries::LibraryBase> GetLibrary() const override { return this->library; }

            virtual void InitializeDevices() override;

            virtual bool EnableDevice(const unsigned int id) override;
            virtual bool DisableDevice(const unsigned int id) override;
            virtual bool ResetDevice(const unsigned int id) override;

            // Custom settings
            void SetRange(const int outMin, const int outMax, const int inMin, const int inMax);
            void SetRestoreLightsWhenOffEnabled(const bool enabled);
            void SetG110WorkaroundEnabled(const bool enabled);

        protected:
            virtual bool PushColorsToDevice(const unsigned int id, const std::vector<timelines::LightColor>& colors) override;

        private:
            std::shared_ptr<libraries::LibraryLogitech> library = std::make_shared<libraries::LibraryLogitech>();

            // Custom settings
            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 100;
            bool restoreLightsWhenOffEnabled = false;
            bool g110WorkaroundEnabled = false;
        };

    }
}

#pragma warning(pop)

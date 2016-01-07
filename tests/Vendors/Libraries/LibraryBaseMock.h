#pragma once

// Standard includes
#include <string>

// Project includes
#include "../../../src/Vendors/Libraries/LibraryBase.h"


namespace lightfx_tests {
    namespace vendors {
        namespace libraries {

            class LibraryBaseMock : public lightfx::vendors::libraries::LibraryBase {

            public:
                using LibraryBase::IsLibraryInitialized;
                virtual bool IsLibraryAvailable() const override { return false; }
                virtual bool InitializeLibrary() override { return false; }
                virtual bool ReleaseLibrary() override { return false; }

                virtual lightfx::Version GetLibraryVerion() const override { return{}; }

            };

        }
    }
}

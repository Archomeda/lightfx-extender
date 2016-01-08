#pragma once

// Project includes
#include "../../Version.h"

// API exports
#include "../../Common/ApiExports.h"


namespace lightfx {
    namespace vendors {
        namespace libraries {

            class LFXE_API LibraryBase {

            public:
                LibraryBase() {}
                virtual ~LibraryBase() {}

                bool IsLibraryInitialized() const {
                    return this->isInitialized;
                }
                virtual bool IsLibraryAvailable() const = 0;
                virtual bool InitializeLibrary() = 0;
                virtual bool ReleaseLibrary() = 0;

                virtual lightfx::Version GetLibraryVersion() const = 0;

            protected:
                bool isInitialized = false;

            };

        }
    }
}

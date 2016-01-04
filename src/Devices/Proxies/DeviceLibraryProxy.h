#pragma once

// API exports
#include "../../Common/ApiExports.h"


namespace lightfx {
    namespace devices {
        namespace proxies {

            class LFXE_API DeviceLibraryProxy {

            public:
                virtual ~DeviceLibraryProxy();

                bool IsLoaded();

                virtual bool Load();
                virtual bool Unload();

            protected:
                void SetLoaded(const bool loaded);

            private:
                bool isLoaded = false;

            };

        }
    }
}

#pragma once

// Standard includes
#include <string>

// Project includes
#include "../LightFXExtender.h"
#include "Version.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    class LightFXExtender;

    namespace managers {

        class LFXE_API UpdateManager {

        public:
            UpdateManager(const LightFXExtender* lightFXExtender);
            ~UpdateManager();

            Version GetCurrentVersion();
            Version GetLiveVersion();

            std::wstring GetDownloadPageUrl();

        };

    }
}

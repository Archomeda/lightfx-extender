#pragma once

// Standard includes
#include <string>

// Project includes
#include "Manager.h"
#include "Version.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    class LightFXExtender;

    namespace managers {

        class LFXE_API UpdateManager : public Manager < UpdateManager > {

        public:
            UpdateManager() {}

            Version GetCurrentVersion();
            Version GetLiveVersion();

            std::wstring GetDownloadPageUrl();

        protected:
            const std::wstring GitHub_ApiTagsUrl = L"https://api.github.com/repos/Archomeda/lightfx-extender/tags";
            const std::wstring GitHub_ReleasesUrl = L"https://github.com/Archomeda/lightfx-extender/releases";

        };

    }
}

#pragma warning(pop)

#pragma once

// Standard includes
#include <string>
#include <thread>
#include <vector>

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
            ~UpdateManager();

            void CheckAsync();

            Version GetCurrentVersion();
            std::pair<Version, std::wstring> GetLiveVersion();

            std::wstring GetDownloadPageUrl();
            bool UpdateLightFX(const std::wstring& downloadUrl);

        protected:
            const std::wstring GitHub_ApiReleasesUrl = L"https://api.github.com/repos/Archomeda/lightfx-extender/releases";
            const std::wstring GitHub_ReleasesUrl = L"https://github.com/Archomeda/lightfx-extender/releases";

            std::vector<char> DownloadFromUrl(const std::wstring& url);

        private:
            void CheckForUpdate();
            std::thread updateCheckerThread;

        };

    }
}

#pragma warning(pop)

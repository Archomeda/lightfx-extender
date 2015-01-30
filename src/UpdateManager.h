#pragma once

// Project includes
#include "Version.h"
#include "VersionInfo.h"


namespace lightfx {
    
    class UpdateManager {

    public:
        Version GetCurrentVersion() { return Version::FromString(CURRENT_VERSION); }
        Version GetLiveVersion();

        std::wstring GetDownloadPageUrl() { return this->GitHub_ReleasesUrl; }

    protected:
        const std::wstring GitHub_ApiTagsUrl = L"https://api.github.com/repos/Archomeda/lightfx-extender/tags";
        const std::wstring GitHub_ReleasesUrl = L"https://github.com/Archomeda/lightfx-extender/releases";

    };

}

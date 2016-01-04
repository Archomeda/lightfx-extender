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

// Forward declare mz stuff from miniz.c
typedef struct mz_zip_archive_tag mz_zip_archive;


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    class LightFXExtender;

    namespace managers {

        struct Release {
            Version version;
            std::wstring downloadUrl;
            std::wstring releaseNotesUrl;
        };

        class LFXE_API UpdateManager : public Manager < UpdateManager > {

        public:
            UpdateManager() {}
            ~UpdateManager();

            void CheckAsync();

            Version GetCurrentVersion();
            Release GetLatestRelease();

            bool UpdateLightFX(const std::wstring& downloadUrl, const Version& version);
            static void InstallNewVersion(const std::wstring& archiveFilename, const Version& version);
            static void InstallNewVersion(const std::vector<char>& archiveData, const Version& version);

        protected:
            std::vector<char> DownloadFromUrl(const std::wstring& url);
            static void InstallNewVersion(mz_zip_archive* const archive, const Version& version);
            static void InstallNewFile(mz_zip_archive* const archive, const int id, const std::string& folder, const std::string& filename);
            static bool UpdateVersionRegistry(const Version& version);

        private:
            void CheckForUpdate();
            std::thread updateCheckerThread;

        };

    }
}

#pragma warning(pop)

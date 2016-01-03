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
typedef unsigned int mz_uint;


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

            bool UpdateLightFX(const std::wstring& downloadUrl);
            static void InstallNewVersion(mz_zip_archive* const archive, mz_uint* const index);

        protected:
            std::vector<char> DownloadFromUrl(const std::wstring& url);
            static void InstallNewFile(mz_zip_archive* const archive, const int id, const std::string& folder, const std::string& filename);

        private:
            void CheckForUpdate();
            std::thread updateCheckerThread;

        };

    }
}

#pragma warning(pop)

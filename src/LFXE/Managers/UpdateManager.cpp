#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "UpdateManager.h"

// Standard includes
#include <regex>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

// 3rd party includes
#include "rapidjson/document.h"
#include "miniz.c"

// Project includes
#include "../VersionInfo.h"
#include "../LightFXExtender.h"
#include "ConfigManager.h"
#include "TrayManager.h"
#include "../Config/MainConfigFile.h"
#include "../Utils/FileIO.h"
#include "../Utils/Log.h"
#include "../Utils/String.h"
#include "../Utils/Windows.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"UpdateManager - ") + message)

#define GITHUB_RELEASEURL_API L"https://api.github.com/repos/Archomeda/lightfx-extender/releases"
#define GITHUB_RELEASEURL L"https://github.com/Archomeda/lightfx-extender/releases"


using namespace std;
using namespace rapidjson;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API UpdateManager::~UpdateManager() {
            // Join update thread in order to close properly, otherwise we might get crashes when trying to close games (e.g. Guild Wars 2)
            this->updateCheckerThread.join();
        }


        LFXE_API void UpdateManager::CheckAsync() {
            this->updateCheckerThread = thread(&UpdateManager::CheckForUpdate, this);
        }

        LFXE_API Version UpdateManager::GetCurrentVersion() {
            return Version::FromString(CURRENT_VERSION);
        }

        LFXE_API pair<Version, wstring> UpdateManager::GetLatestRelease() {
            vector<char> releaseData;
            try {
                releaseData = this->DownloadFromUrl(GITHUB_RELEASEURL_API);
            } catch (const exception& e) {
                LOG(LogLevel::Error, L"Error while downloading latest release info: " + string_to_wstring(e.what()));
            }

            Version version;
            string downloadUrl;
            string releaseString(releaseData.begin(), releaseData.end());
            Document releaseJson;
            releaseJson.Parse<0>(releaseString.c_str());
            if (releaseJson.IsArray()) {
                // We get the information from the first available release 
                // This is to capture a possible failure when later releases will have different filenames.
                // If this happens, this will download the latest release possible, and then that version will download a newer release.
                for (SizeType i = 0; i < releaseJson.Size(); ++i) {
                    // Check if this release is not a pre-release first
                    if (releaseJson[i].HasMember("tag_name") && releaseJson[i]["tag_name"].IsString()) {
                        if (releaseJson[i].HasMember("prerelease") && releaseJson[i]["prerelease"].IsBool()) {
                            if (releaseJson[i]["prerelease"].GetBool()) {
                                //TODO: Skip pre-releases for now, include an optional configuration to download them first
                                continue;
                            }
                        }
                    }

                    // Get the version and asset download URL
                    bool isValid = false;
                    if (releaseJson[i].HasMember("assets") && releaseJson[i]["assets"].IsArray()) {
                        auto& assets = releaseJson[i]["assets"];
                        regex re("LightFX-Extender-full_v(\\d+\\.\\d+\\.\\d+\\.\\d+)\\.zip");
                        smatch match;
                        for (SizeType j = 0; j < assets.Size(); ++j) {
                            if (assets[j].HasMember("name") && assets[j]["name"].IsString()) {
                                string assetName = assets[j]["name"].GetString();
                                if (regex_search(assetName, match, re) && match.size() > 1) {
                                    version = Version::FromString(releaseJson["tag_name"].GetString());
                                    if (assets[j].HasMember("browser_download_url") && assets[j]["browser_download_url"].IsString()) {
                                        downloadUrl = assets[j]["browser_download_url"].GetString();
                                        isValid = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (isValid) {
                        break;
                    }
                }
            }

            return{ version, string_to_wstring(downloadUrl) };
        }

        LFXE_API bool UpdateManager::UpdateLightFX(const wstring& downloadUrl) {
            if (downloadUrl.empty()) {
                return false;
            }
            wstring pathw = GetDataStorageFolder();
            string path = wstring_to_string(pathw);

            vector<char> releaseZip;
            try {
                releaseZip = this->DownloadFromUrl(downloadUrl);
            } catch (const exception& e) {
                LOG(LogLevel::Error, L"Error while downloading latest release archive: " + string_to_wstring(e.what()));
                return false;
            }

            mz_zip_archive archive;
            memset(&archive, 0, sizeof(archive));
            if (!mz_zip_reader_init_mem(&archive, &releaseZip[0], releaseZip.size(), 0)) {
                LOG(LogLevel::Error, L"Failed to open the downloaded archive");
                return false;
            }

            mz_zip_archive_file_stat filestat;
            if (!mz_zip_reader_file_stat(&archive, 0, &filestat)) {
                LOG(LogLevel::Error, L"Failed to read the downloaded archive");
                mz_zip_reader_end(&archive);
                return false;
            }

            mz_uint numFiles = mz_zip_reader_get_num_files(&archive);
            for (mz_uint i = 0; i < numFiles; ++i) {
                unique_ptr<char[]> buff(new char[1024]);
                mz_zip_reader_get_filename(&archive, i, buff.get(), 1024);
                string filename(buff.get());
                if (filename.compare(0, 4, "bin/") == 0) {
                    // Bin file, extract to root
                    filename = path + filename.substr(4);
                } else if (filename.compare(0, 4, "x86/") == 0) {
                    // Additional x86 files, extract to relative folder
                    if (!CreateDirectoryW((pathw + L"\\x86").c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
                        LOG(LogLevel::Error, L"Failed to create the x86 directory in the AppData storage directory");
                        mz_zip_reader_end(&archive);
                        return false;
                    }
                    filename = path + filename;
                } else if (filename.compare(0, 4, "x64/") == 0) {
                    // Additional x64 files, extract to relative folder
                    if (!CreateDirectoryW((pathw + L"\\x64").c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
                        LOG(LogLevel::Error, L"Failed to create the x64 directory in the AppData storage directory");
                        mz_zip_reader_end(&archive);
                        return false;
                    }
                    filename = path + filename;
                } else {
                    // Other file, skip
                    continue;
                }

                try {
                    this->InstallNewFile(&archive, i, filename);
                } catch (const exception& e) {
                    LOG(LogLevel::Error, L"Error while installing new file " + string_to_wstring(filename) + L": " + string_to_wstring(e.what()));
                    Log::LogLastWindowsErrorAsync();
                    mz_zip_reader_end(&archive);
                    return false;
                }
            }

            mz_zip_reader_end(&archive);
            return true;
        }

        LFXE_API void UpdateManager::InstallNewFile(mz_zip_archive* archive, const int id, const std::string& filename) {
            wstring targetFilename = GetDataStorageFolder() + L"\\" + string_to_wstring(filename);
            wstring backupFilename = targetFilename + L".bak";
            wstring tempFilename = targetFilename + L".tmp";

            if (!mz_zip_reader_extract_to_file(archive, id, wstring_to_string(tempFilename).c_str(), 0)) {
                throw exception(("Failed to extract " + filename + " from the archive").c_str());
            }

            if (FileExists(backupFilename) && !DeleteFileW(backupFilename.c_str())) {
                throw exception(("Failed to remove the backed up file " + wstring_to_string(backupFilename)).c_str());
            }
            if (FileExists(targetFilename) && !DeleteFileW(targetFilename.c_str())) {
                // Could not delete the target file, assume the file is in use and let's try to move it
                if (!MoveFileW(targetFilename.c_str(), backupFilename.c_str())) {
                    // Could not move the file either, fail
                    throw exception(("Failed to rename the file " + wstring_to_string(targetFilename) + " to " + wstring_to_string(backupFilename)).c_str());
                }
            }
            if (!MoveFileW(tempFilename.c_str(), targetFilename.c_str())) {
                // Could not move the downloaded file to the target location
                throw exception(("Failed to rename the new file " + wstring_to_string(tempFilename) + " to " + wstring_to_string(targetFilename) +
                    "; LightFX Extender will not work properly until you manually rename this file").c_str());
            }
        }

        LFXE_API vector<char> UpdateManager::DownloadFromUrl(const wstring& url) {
            vector<char> data = {};
            HINTERNET hSession = InternetOpenW(L"LightFX Extender Update Manager", 0, NULL, NULL, 0);
            if (hSession == NULL) {
                throw exception("Failed to open session");
            }

            HINTERNET hOpenUrl = InternetOpenUrlW(hSession, url.c_str(), NULL, 0, 1, 1);
            if (hOpenUrl == NULL) {
                InternetCloseHandle(hOpenUrl);
                throw exception("Failed to open URL");
            }

            char* buffer = new char[4096];
            DWORD bytesRead = 0;
            while (true) {
                if (InternetReadFile(hOpenUrl, buffer, 4096, &bytesRead)) {
                    if (bytesRead == 0) {
                        break;
                    }
                    data.insert(data.end(), buffer, buffer + bytesRead);
                } else {
                    delete[] buffer;
                    InternetCloseHandle(hOpenUrl);
                    InternetCloseHandle(hSession);
                    throw exception("Failed to read from URL");
                }
            }
            delete[] buffer;
            InternetCloseHandle(hOpenUrl);
            InternetCloseHandle(hSession);

            return data;
        }


        LFXE_API void UpdateManager::CheckForUpdate() {
            Version currentVersion = this->GetCurrentVersion();
            LOG(LogLevel::Debug, L"Checking for updates...");

            auto live = this->GetLatestRelease();
            Version liveVersion = live.first;
            wstring downloadUrl = live.second;
            if (liveVersion > currentVersion) {
                wstring newVersionString = liveVersion.ToString();
                LOG(LogLevel::Info, L"A newer version is available: " + newVersionString);
                if (this->GetLightFXExtender()->GetConfigManager()->GetMainConfig()->AutoUpdatesEnabled) {
                    if (this->UpdateLightFX(downloadUrl)) {
                        LOG(LogLevel::Info, L"LightFX Extender has been updated automatically to " + newVersionString);
                        LOG(LogLevel::Info, L"The changes will be applied the next time you run LightFX Extender");
                    } else {
                        LOG(LogLevel::Warning, L"LightFX Extender could not be updated automatically, see " + GITHUB_RELEASEURL + L" for downloads");
                        this->GetLightFXExtender()->GetTrayManager()->SetUpdateNotification(newVersionString, GITHUB_RELEASEURL);
                    }
                } else {
                    LOG(LogLevel::Info, L"Auto updates are disabled, see " + GITHUB_RELEASEURL + L" for downloads");
                    this->GetLightFXExtender()->GetTrayManager()->SetUpdateNotification(newVersionString, GITHUB_RELEASEURL);
                }
            } else {
                LOG(LogLevel::Debug, L"No update available");
            }
        }

    }
}

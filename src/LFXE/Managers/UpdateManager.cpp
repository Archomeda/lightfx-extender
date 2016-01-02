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
#include "../Exceptions/AccessDeniedException.h"
#include "../Utils/FileIO.h"
#include "../Utils/Log.h"
#include "../Utils/String.h"
#include "../Utils/Windows.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"UpdateManager - ") + message)

#define GITHUB_RELEASEURL_API L"https://api.github.com/repos/Archomeda/lightfx-extender/releases"


using namespace std;
using namespace rapidjson;
using namespace lightfx::exceptions;
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

        LFXE_API Release UpdateManager::GetLatestRelease() {
            vector<char> releaseData;
            try {
                releaseData = this->DownloadFromUrl(GITHUB_RELEASEURL_API);
            } catch (const exception& e) {
                LOG(LogLevel::Error, L"Error while downloading latest release info: " + string_to_wstring(e.what()));
            }

            Version version;
            string downloadUrl;
            string releaseNotesUrl;

            string releaseString(releaseData.begin(), releaseData.end());
            Document releaseJson;
            releaseJson.Parse<0>(releaseString.c_str());
            if (releaseJson.IsArray()) {
                // We get the information from the first available release 
                // This is to capture a possible failure when later releases will have different filenames.
                // If this happens, this will download the latest release possible, and then that version will download a newer release.
                for (SizeType i = 0; i < releaseJson.Size(); ++i) {
                    // Check if this release is not a pre-release first
                    if (releaseJson[i].HasMember("prerelease") && releaseJson[i]["prerelease"].IsBool()) {
                        if (releaseJson[i]["prerelease"].GetBool()) {
                            //TODO: Skip pre-releases for now, include an optional configuration to download them first
                            continue;
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
                                    if (releaseJson[i].HasMember("tag_name") && releaseJson[i]["tag_name"].IsString()) {
                                        version = Version::FromString(releaseJson["tag_name"].GetString());
                                    }
                                    if (releaseJson[i].HasMember("html_url") && releaseJson[i]["html_url"].IsString()) {
                                        releaseNotesUrl = releaseJson["html_url"].GetString();
                                    }
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

            Release rel;
            rel.version = version;
            rel.downloadUrl = string_to_wstring(downloadUrl);
            rel.releaseNotesUrl = string_to_wstring(releaseNotesUrl);
            return rel;
        }

        LFXE_API bool UpdateManager::UpdateLightFX(const wstring& downloadUrl) {
            if (downloadUrl.empty()) {
                return false;
            }
            wstring pathw = GetProgramFolder();
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
                    filename = filename.substr(4);
                } else if (filename.compare(0, 4, "x86/") == 0 || filename.compare(0, 4, "x64/") == 0) {
                    // Additional x86 or x64 files, extract to relative folder
                    filename = filename;
                } else {
                    // Other file, skip
                    continue;
                }

                // Split the folder part from the filename
                size_t splitPos = filename.find_last_of("/\\");
                string folder;
                if (splitPos != string::npos) {
                    folder = filename.substr(0, splitPos + 1);
                    filename = filename.substr(splitPos + 1);
                }

                // Try installing the new file
                try {
                    this->InstallNewFile(&archive, i, folder, filename);
                } catch (const AccessDeniedException&) {
                    if (!IsProcessElevated()) {
                        // Process is not elevated, and therefore we assume that we don't have enough permissions
                        //TODO: Try to run a separate elevated process and resume installing
                        LOG(LogLevel::Error, L"Error while installing new file " + string_to_wstring(filename) + L": Access denied, process was not elevated");
                        mz_zip_reader_end(&archive);
                        return false;
                    } else {
                        LOG(LogLevel::Error, L"Error while installing new file " + string_to_wstring(filename) + L": Access denied");
                        mz_zip_reader_end(&archive);
                        return false;
                    }
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

        LFXE_API void UpdateManager::InstallNewFile(mz_zip_archive* archive, const int id, const string& folder, const string& filename) {
            wstring folderW = string_to_wstring(folder);
            wstring filenameW = string_to_wstring(filename);
            wstring targetFolder = GetProgramFolder() + L"\\" + folderW;
            wstring targetFilename = targetFolder + filenameW;
            wstring backupFilename = targetFilename + L".bak";
            wstring tempFolder = GetDataStorageFolder();
            wstring tempFilename = tempFolder + L"\\" + filenameW + L".tmp";

            // Create required folders
            if (!CreateDirIfNotExists(tempFolder)) {
                throw exception(("Failed to create the temporary folder " + wstring_to_string(tempFolder)).c_str());
            }
            if (!CreateDirIfNotExists(targetFolder)) {
                throw exception(("Failed to create the target folder " + wstring_to_string(targetFolder)).c_str());
            }

            // Extract the file to a temporary location
            if (!mz_zip_reader_extract_to_file(archive, id, wstring_to_string(tempFilename).c_str(), 0)) {
                if (GetLastError() == ERROR_ACCESS_DENIED) {
                    throw AccessDeniedException();
                } else {
                    throw exception(("Failed to extract " + filename + " from the archive").c_str());
                }
            }

            // Remove the backed up file if it exists
            if (!RemoveFile(backupFilename)) {
                throw exception(("Failed to remove the backed up file " + wstring_to_string(backupFilename)).c_str());
            }

            // Delete the existing target file if it exists
            try {
                if (!RemoveFile(targetFilename)) {
                    throw exception(("Failed to remove the existing file " + wstring_to_string(targetFilename)).c_str());
                }
            } catch (const AccessDeniedException&) {
                // Assume that this access denied exception is caused by the fact that the file is in use
                // Let's try to move it
                if (!RenameFile(targetFilename, backupFilename)) {
                    throw exception(("Failed to rename the file " + wstring_to_string(targetFilename) + " to " + wstring_to_string(backupFilename)).c_str());
                }
            }

            // Move the new file to its intended target
            if (!RenameFile(tempFilename, targetFilename)) {
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

            auto release = this->GetLatestRelease();
            if (release.version > currentVersion) {
                wstring newVersionString = release.version.ToString();
                LOG(LogLevel::Info, L"A newer version is available: " + newVersionString);
                if (this->GetLightFXExtender()->GetConfigManager()->GetMainConfig()->AutoUpdatesEnabled) {
                    if (this->UpdateLightFX(release.downloadUrl)) {
                        LOG(LogLevel::Info, L"LightFX Extender has been updated automatically to " + newVersionString);
                        LOG(LogLevel::Info, L"The changes will be applied the next time you run LightFX Extender");
                        this->GetLightFXExtender()->GetTrayManager()->SetUpdateInstalledNotification(newVersionString, release.releaseNotesUrl);
                    } else {
                        LOG(LogLevel::Warning, L"LightFX Extender could not be updated automatically, see " + release.releaseNotesUrl + L" for downloads");
                        this->GetLightFXExtender()->GetTrayManager()->SetUpdateAvailableNotification(newVersionString, release.releaseNotesUrl);
                    }
                } else {
                    LOG(LogLevel::Info, L"Auto updates are disabled, see " + release.releaseNotesUrl + L" for downloads");
                    this->GetLightFXExtender()->GetTrayManager()->SetUpdateAvailableNotification(newVersionString, release.releaseNotesUrl);
                }
            } else {
                LOG(LogLevel::Debug, L"No update available");
            }
        }

    }
}

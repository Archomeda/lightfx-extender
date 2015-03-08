#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "UpdateManager.h"

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
#include "LogManager.h"
#include "TrayManager.h"
#include "../Config/MainConfigFile.h"
#include "../Utils/FileIO.h"
#include "../Utils/String.h"
#include "../Utils/Windows.h"


#define LOG(logLevel, line) if (this->GetLightFXExtender() != nullptr) { this->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"UpdateManager - ") + line); }
#define LOGWINERROR() if (this->GetLightFXExtender() != nullptr) { this->GetLightFXExtender()->GetLogManager()->LogLastWindowsError(); }

#ifdef _WIN64
#define PLATFORM "x64"
#else
#define PLATFORM "x86"
#endif

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

        LFXE_API pair<Version, wstring> UpdateManager::GetLiveVersion() {
            vector<char> data = this->DownloadFromUrl(this->GitHub_ApiReleasesUrl);
            string jsonString(data.begin(), data.end());
            Document json;
            json.Parse<0>(jsonString.c_str());
            if (json.IsArray()) {
                for (SizeType i = 0; i < json.Size(); ++i) {
                    if (json[i].HasMember("tag_name") && json[i]["tag_name"].IsString()) {
                        if (json[i].HasMember("prerelease") && json[i]["prerelease"].IsBool()) {
                            if (json[i]["prerelease"].GetBool()) {
                                // TODO: Skip pre-releases for now, include an optional configuration to download them first
                                continue;
                            }
                        }
                        string tagName = json[i]["tag_name"].GetString();
                        string assetUrl;
                        if (json[i].HasMember("assets") && json[i]["assets"].IsArray()) {
                            auto& assets = json[i]["assets"];
                            for (SizeType j = 0; j < assets.Size(); ++j) {
                                if (assets[j].HasMember("name") && assets[j]["name"].IsString()) {
                                    string assetName = assets[j]["name"].GetString();
                                    string substring = string(PLATFORM) + ".zip";
                                    if (assetName.compare(assetName.length() - substring.length(), substring.length(), substring) == 0) {
                                        if (assets[j].HasMember("browser_download_url") && assets[j]["browser_download_url"].IsString()) {
                                            assetUrl = assets[j]["browser_download_url"].GetString();
                                        }
                                    }
                                }
                            }
                        }

                        if (assetUrl != "" && tagName.compare(0, 1, "v") == 0) {
                            return{ Version::FromString(tagName.substr(1)), string_to_wstring(assetUrl) };
                        }
                    }
                }
            }

            return{};
        }


        LFXE_API wstring UpdateManager::GetDownloadPageUrl() {
            return this->GitHub_ReleasesUrl;
        }

        LFXE_API bool UpdateManager::UpdateLightFX(const wstring& downloadUrl) {
            if (downloadUrl == L"") {
                return false;
            }
            vector<char> newVersionZip = this->DownloadFromUrl(downloadUrl);
            if (newVersionZip.size() == 0) {
                return false;
            }

            mz_zip_archive archive;
            memset(&archive, 0, sizeof(archive));
            if (!mz_zip_reader_init_mem(&archive, &newVersionZip[0], newVersionZip.size(), 0)) {
                LOG(LogLevel::Error, L"Could not open the downloaded archive");
                return false;
            }

            mz_zip_archive_file_stat filestat;
            if (!mz_zip_reader_file_stat(&archive, 0, &filestat)) {
                LOG(LogLevel::Error, L"Could not read the downloaded archive");
                mz_zip_reader_end(&archive);
                return false;
            }

            int file_index = mz_zip_reader_locate_file(&archive, "LightFX.dll", NULL, 0);
            string tempFileName = wstring_to_string(GetDataStorageFolder() + L"/LightFX.dll");
            if (!mz_zip_reader_extract_to_file(&archive, file_index, tempFileName.c_str(), 0)) {
                LOG(LogLevel::Error, L"Could not extract LightFX.dll from the downloaded archive");
                mz_zip_reader_end(&archive);
                return false;
            }

            wstring dllFileName = GetDllName();
            wstring newDllFileName = string_to_wstring(tempFileName);
            LOG(LogLevel::Debug, L"Downloaded new LightFX.dll to " + newDllFileName);

            if (FileExists(dllFileName + L".bak")) {
                if (!DeleteFileW((dllFileName + L".bak").c_str())) {
                    LOG(LogLevel::Error, L"Could not remove the back-upped DLL file from the previous version");
                    LOGWINERROR();
                    return false;
                }
            }
            if (MoveFileW(dllFileName.c_str(), (dllFileName + L".bak").c_str()) == FALSE) {
                LOG(LogLevel::Error, L"Could not rename the DLL file");
                LOGWINERROR();
                return false;
            }
            if (MoveFileW(newDllFileName.c_str(), dllFileName.c_str()) == FALSE) {
                LOG(LogLevel::Error, L"Could not move the new DLL into its intended folder, LightFX Extender will not work unless you manually move " +
                    newDllFileName + L" to " + dllFileName);
                LOGWINERROR();
                return false;
            }

            return true;
        }


        LFXE_API vector<char> UpdateManager::DownloadFromUrl(const wstring& url) {
            vector<char> data = {};
            HINTERNET hSession = InternetOpenW(L"LightFX Extender Update Manager", 0, NULL, NULL, 0);
            if (hSession == NULL) {
                LOG(LogLevel::Error, L"Failed to open session");
                return data;
            }

            HINTERNET hOpenUrl = InternetOpenUrlW(hSession, url.c_str(), NULL, 0, 1, 1);
            if (hOpenUrl == NULL) {
                LOG(LogLevel::Error, L"Failed to open URL");
                InternetCloseHandle(hOpenUrl);
                return data;
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
                    LOG(LogLevel::Error, L"Failed to read from URL");
                    delete[] buffer;
                    InternetCloseHandle(hOpenUrl);
                    InternetCloseHandle(hSession);
                    return data;
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

            auto live = this->GetLiveVersion();
            Version liveVersion = live.first;
            wstring downloadUrl = live.second;
            if (liveVersion > currentVersion) {
                wstring newVersionString = liveVersion.ToString();
                LOG(LogLevel::Info, L"A newer version is available: " + newVersionString);
                if (this->GetLightFXExtender()->GetConfigManager()->GetMainConfig()->AutoUpdatesEnabled) {
                    if (this->UpdateLightFX(downloadUrl)) {
                        LOG(LogLevel::Info, L"LightFX Extender has been updated automatically to " + newVersionString);
                        LOG(LogLevel::Info, L"The changes will be applied the next time you run LightFX Extender");
                        LOG(LogLevel::Info, L"If, for some reason, the newer version will not start, please restore LightFX.dll.bak to LightFX.dll (and disable auto updates in the config and wait for a fix if this keeps on happening)");
                    } else {
                        wstring newVersionUrl = this->GetDownloadPageUrl();
                        LOG(LogLevel::Warning, L"LightFX Extender could not be updated automatically, see " + newVersionUrl + L" for downloads");
                        this->GetLightFXExtender()->GetTrayManager()->SetUpdateNotification(newVersionString, newVersionUrl);
                    }
                } else {
                    wstring newVersionUrl = this->GetDownloadPageUrl();
                    LOG(LogLevel::Info, L"Auto updates are disabled, see " + newVersionUrl + L" for downloads");
                    this->GetLightFXExtender()->GetTrayManager()->SetUpdateNotification(newVersionString, newVersionUrl);
                }
            } else {
                LOG(LogLevel::Debug, L"No update available");
            }
        }

    }
}
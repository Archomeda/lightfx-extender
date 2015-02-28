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

// Project includes
#include "../VersionInfo.h"
#include "../LightFXExtender.h"
#include "LogManager.h"
#include "TrayManager.h"


#define LOG(logLevel, line) if (this->GetLightFXExtender() != nullptr) { this->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"UpdateManager - ") + line); }

using namespace std;
using namespace rapidjson;

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

        LFXE_API Version UpdateManager::GetLiveVersion() {
            HINTERNET hSession = InternetOpenW(L"LightFX Extender Update Checker", 0, NULL, NULL, 0);
            if (hSession == NULL) {
                LOG(LogLevel::Error, L"Failed to open session");
                return Version();
            }

            HINTERNET hOpenUrl = InternetOpenUrlW(hSession, this->GitHub_ApiTagsUrl.c_str(), NULL, 0, 1, 1);
            if (hOpenUrl == NULL) {
                LOG(LogLevel::Error, L"Failed to open URL");
                InternetCloseHandle(hOpenUrl);
                return Version();
            }

            string data;

            char* buffer = new char[1025];
            DWORD bytesRead = 0;
            while (true) {
                if (InternetReadFile(hOpenUrl, buffer, 1024, &bytesRead)) {
                    if (bytesRead == 0) {
                        break;
                    }
                    buffer[bytesRead] = 0;
                    data.append(buffer);
                } else {
                    LOG(LogLevel::Error, L"Failed to read web page");
                    delete[] buffer;
                    InternetCloseHandle(hOpenUrl);
                    InternetCloseHandle(hSession);
                    return Version();
                }
            }
            delete[] buffer;
            InternetCloseHandle(hOpenUrl);
            InternetCloseHandle(hSession);

            Document json;
            json.Parse<0>(data.c_str());
            if (json.IsArray()) {
                for (SizeType i = 0; i < json.Size(); ++i) {
                    if (json[i].HasMember("name") && json[i]["name"].IsString()) {
                        string tagName = json[i]["name"].GetString();
                        if (tagName.compare(0, 1, "v") == 0) {
                            return Version::FromString(tagName.substr(1));
                        }
                    }
                }
            }

            return Version();
        }


        LFXE_API wstring UpdateManager::GetDownloadPageUrl() {
            return this->GitHub_ReleasesUrl;
        }


        LFXE_API void UpdateManager::CheckForUpdate() {
            Version currentVersion = this->GetCurrentVersion();
            LOG(LogLevel::Info, L"Checking for updates... (current version: " +
                to_wstring(currentVersion.GetMajor()) + L"." + to_wstring(currentVersion.GetMinor()) + L"." + to_wstring(currentVersion.GetBuild()) + L")");

            Version liveVersion = this->GetLiveVersion();
            if (liveVersion > currentVersion) {
                wstring newVersionString = to_wstring(liveVersion.GetMajor()) + L"." + to_wstring(liveVersion.GetMinor()) + L"." + to_wstring(liveVersion.GetBuild());
                wstring newVersionUrl = this->GetDownloadPageUrl();
                LOG(LogLevel::Info, L"A newer version is available: " + newVersionString);
                LOG(LogLevel::Info, L"See " + newVersionUrl + L" for downloads");
                this->GetLightFXExtender()->GetTrayManager()->SetUpdateNotification(newVersionString, newVersionUrl);
            } else {
                LOG(LogLevel::Info, L"No update available");
            }
        }

    }
}
#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "UpdateManager.h"

// Standard includes
#include <fstream>
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
#include "../Exceptions/MzFileStatException.h"
#include "../Exceptions/MzReaderInitException.h"
#include "../Utils/FileIO.h"
#include "../Utils/Log.h"
#include "../Utils/Registry.h"
#include "../Utils/String.h"
#include "../Utils/Windows.h"


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
                LOG_ERROR(L"Error while downloading latest release info: " + string_to_wstring(e.what()));
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

        LFXE_API bool UpdateManager::UpdateLightFX(const wstring& downloadUrl, const Version& version) {
            if (downloadUrl.empty()) {
                return false;
            }
            wstring pathw = GetProgramFolder();
            string path = wstring_to_string(pathw);

            vector<char> releaseZip;
            try {
                releaseZip = this->DownloadFromUrl(downloadUrl);
            } catch (const exception& e) {
                LOG_ERROR(L"Error while downloading latest release archive: " + string_to_wstring(e.what()));
                return false;
            }

            // If LightFX Extender was installed through the installer, we need elevated access
            if (!IsProcessElevated()) {
                // Elevate
                wstring id = GetRegKeyString(HKEY_LOCAL_MACHINE, REGKEY_LFXE, L"AppId", L"", KEY_WOW64_64KEY);
                if (!id.empty()) {
                    MessageBoxA(NULL, (string("There is an update available for LightFX Extender, but we need permissions in order to update.\r\n\r\n") +
                        "After pressing OK, you will be asked to accept the permissions through the Windows elevation prompt. " +
                        "Please accept in order to update LightFX Extender.").c_str(), "LightFX Extender", MB_OK | MB_ICONINFORMATION);

                    // Save the archive to a temporary location
                    wstring tempFilename = GetDataStorageFolder() + L"\\download.zip";
                    ofstream ofs(tempFilename, ios::out | ios::binary);
                    copy(releaseZip.begin(), releaseZip.end(), ostreambuf_iterator<char>(ofs));
                    ofs.close();

                    // Set up process information
                    wstring dllName(GetDllName());
                    wstring parameters(L"\"" + dllName + L"\",LFXE_UpdateInstallation " + version.ToString());
                    SHELLEXECUTEINFOW sei = { sizeof(sei) };
                    sei.lpVerb = L"runas";
                    sei.lpFile = L"rundll32.exe";
                    sei.lpParameters = parameters.c_str();
                    sei.hwnd = NULL;
                    sei.nShow = SW_SHOWDEFAULT;

                    // Start process
                    if (ShellExecuteExW(&sei)) {
                        return true;
                    } else {
                        RemoveFile(tempFilename);
                        LOG_ERROR(L"Error while updating LightFX Extender: Access denied, process could not be elevated");
                        LOG_WINERROR();
                        return false;
                    }
                }
            } else {
                // We are elevated, continue with installing
                try {
                    this->InstallNewVersion(releaseZip, version);
                } catch (const MsFileStatException&) {
                    LOG_ERROR(L"Error while updating LightFX Extender: Failed to read the downloaded archive");
                    return false;
                } catch (const AccessDeniedException&) {
                    LOG_ERROR(L"Error while updating LightFX Extender: Access denied");
                    return false;
                } catch (const exception& e) {
                    LOG_ERROR(L"Error while updating LightFX Extender: " + string_to_wstring(e.what()));
                    LOG_WINERROR();
                    return false;
                }
            }

            return true;
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

        LFXE_API void UpdateManager::InstallNewVersion(const wstring& archiveFilename, const Version& version) {
            mz_zip_archive archive;
            try {
                memset(&archive, 0, sizeof(archive));
                if (!mz_zip_reader_init_file(&archive, wstring_to_string(archiveFilename.c_str()).c_str(), 0)) {
                    throw MzReaderInitException();
                }
                UpdateManager::InstallNewVersion(&archive, version);
                UpdateManager::UpdateVersionRegistry(version);
            } catch (...) {
                mz_zip_reader_end(&archive);
                throw;
            }
            mz_zip_reader_end(&archive);
        }

        LFXE_API void UpdateManager::InstallNewVersion(const vector<char>& archiveData, const Version& version) {
            mz_zip_archive archive;
            try {
                memset(&archive, 0, sizeof(archive));
                if (!mz_zip_reader_init_mem(&archive, &archiveData[0], archiveData.size(), 0)) {
                    throw MzReaderInitException();
                }
                UpdateManager::InstallNewVersion(&archive, version);
                UpdateManager::UpdateVersionRegistry(version);
            } catch (...) {
                mz_zip_reader_end(&archive);
                throw;
            }
            mz_zip_reader_end(&archive);
        }

        LFXE_API void UpdateManager::InstallNewVersion(mz_zip_archive* const archive, const Version& version) {
            mz_zip_archive_file_stat filestat;
            if (!mz_zip_reader_file_stat(archive, 0, &filestat)) {
                throw MsFileStatException();
            }

            mz_uint numFiles = mz_zip_reader_get_num_files(archive);
            for (mz_uint i = 0; i < numFiles; ++i) {
                unique_ptr<char[]> buff(new char[1024]);
                mz_zip_reader_get_filename(archive, i, buff.get(), 1024);
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
                UpdateManager::InstallNewFile(archive, i, folder, filename);
            }
        }

        LFXE_API void UpdateManager::InstallNewFile(mz_zip_archive* const archive, const int id, const string& folder, const string& filename) {
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

            // Remove the backed up file if it exists
            if (!RemoveFile(backupFilename)) {
                throw exception(("Failed to remove the backed up file " + wstring_to_string(backupFilename)).c_str());
            }

            // Move the current file to the backup file
            if (FileExists(targetFilename) && !RenameFile(targetFilename, backupFilename)) {
                throw exception(("Failed to rename the file " + wstring_to_string(targetFilename) + " to " + wstring_to_string(backupFilename)).c_str());
            }

            // Try to remove the existing target file
            try {
                if (!RemoveFile(backupFilename)) {
                    throw exception(("Failed to remove the existing file " + wstring_to_string(targetFilename)).c_str());
                }
            } catch (const AccessDeniedException&) {
                // Access denied might also be caused by a file that's loaded as a DLL by an external application
                // We can still update the file by renaming it to something different
            }

            // Remove the temporary file if it exists
            if (!RemoveFile(tempFilename)) {
                throw exception(("Failed to remove the previous temporary file " + wstring_to_string(tempFilename)).c_str());
            }

            // Extract the file to a temporary location
            if (!mz_zip_reader_extract_to_file(archive, id, wstring_to_string(tempFilename).c_str(), 0)) {
                if (GetLastError() == ERROR_ACCESS_DENIED) {
                    throw AccessDeniedException();
                } else {
                    throw exception(("Failed to extract " + filename + " from the archive").c_str());
                }
            }

            // Move the new file to its intended target
            if (!RenameFile(tempFilename, targetFilename)) {
                throw exception(("Failed to rename the new file " + wstring_to_string(tempFilename) + " to " + wstring_to_string(targetFilename) +
                    "; LightFX Extender will not work properly until you manually rename this file").c_str());
            }
        }

        LFXE_API bool UpdateManager::UpdateVersionRegistry(const Version& version) {
            wstring id = GetRegKeyString(HKEY_LOCAL_MACHINE, REGKEY_LFXE, L"AppId", L"", KEY_WOW64_64KEY);
            if (!id.empty()) {
                wstring subKey = wstring(REGKEY_UNINSTALL) + L"\\" + id + L"_is1";
                return SetRegKeyString(HKEY_LOCAL_MACHINE, subKey, L"DisplayVersion", version.ToString(), KEY_WOW64_64KEY) &&
                    SetRegKeyDword(HKEY_LOCAL_MACHINE, subKey, L"MajorVersion", version.GetMajor(), KEY_WOW64_64KEY) &&
                    SetRegKeyDword(HKEY_LOCAL_MACHINE, subKey, L"MinorVersion", version.GetMinor(), KEY_WOW64_64KEY);
            }
            return false;
        }

        LFXE_API void UpdateManager::CheckForUpdate() {
            Version currentVersion = this->GetCurrentVersion();
            LOG_DEBUG(L"Checking for updates...");

            auto release = this->GetLatestRelease();
            if (release.version > currentVersion) {
                wstring newVersionString = release.version.ToString();
                LOG_INFO(L"A newer version is available: " + newVersionString);
                if (this->GetLightFXExtender()->GetConfigManager()->GetMainConfig()->AutoUpdatesEnabled) {
                    if (this->UpdateLightFX(release.downloadUrl, release.version)) {
                        LOG_INFO(L"LightFX Extender has been updated automatically to " + newVersionString);
                        LOG_INFO(L"The changes will be applied the next time you run LightFX Extender");
                        this->GetLightFXExtender()->GetTrayManager()->SetUpdateInstalledNotification(newVersionString, release.releaseNotesUrl);
                    } else {
                        LOG_WARNING(L"LightFX Extender could not be updated automatically, see " + release.releaseNotesUrl + L" for downloads");
                        this->GetLightFXExtender()->GetTrayManager()->SetUpdateAvailableNotification(newVersionString, release.releaseNotesUrl);
                    }
                } else {
                    LOG_INFO(L"Auto updates are disabled, see " + release.releaseNotesUrl + L" for downloads");
                    this->GetLightFXExtender()->GetTrayManager()->SetUpdateAvailableNotification(newVersionString, release.releaseNotesUrl);
                }
            } else {
                LOG_DEBUG(L"No update available");
            }
        }

    }
}


// Custom RunDll export for installing files in an elevated process
#ifdef __cplusplus
extern "C" {
#endif

    void CALLBACK LFXE_UpdateInstallation(HWND hWnd, HINSTANCE hInstance, LPSTR lpszCmdLine, int nCmdShow) {
        lightfx::managers::Version version = lightfx::managers::Version::FromString(lpszCmdLine);
        wstring archiveFilename = GetDataStorageFolder() + L"\\download.zip";

        try {
            lightfx::managers::UpdateManager::InstallNewVersion(archiveFilename, version);
            RemoveFile(archiveFilename);
        } catch (...) {
            return;
        }
    }

#ifdef __cplusplus
}
#endif

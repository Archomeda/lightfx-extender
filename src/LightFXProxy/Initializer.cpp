#include "Initializer.h"

// Standard includes
#include <codecvt>
#include <memory>
#include <regex>
#include <vector>

// Windows includes
#include <Windows.h>
#include <ShlObj.h>
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

// 3rd party includes
#include "rapidjson/document.h"
#include "miniz.c"

#define DLERROR_OPENSESSION 1
#define DLERROR_OPENURL 2
#define DLERROR_READURL 3

#define GITHUB_RELEASEURL_API L"https://api.github.com/repos/Archomeda/lightfx-extender/releases/latest"

using namespace std;
using namespace rapidjson;


vector<char> DownloadFromUrl(const wstring& url) {
    vector<char> data = {};
    HINTERNET hSession = InternetOpenW(L"LightFX Extender Update Manager", 0, NULL, NULL, 0);
    if (hSession == NULL) {
        // Failed to open session
        throw DLERROR_OPENSESSION;
    }

    HINTERNET hOpenUrl = InternetOpenUrlW(hSession, url.c_str(), NULL, 0, 1, 1);
    if (hOpenUrl == NULL) {
        // Failed to open URL
        InternetCloseHandle(hOpenUrl);
        throw DLERROR_OPENURL;
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
            // Failed to read from URL
            delete[] buffer;
            InternetCloseHandle(hOpenUrl);
            InternetCloseHandle(hSession);
            throw DLERROR_READURL;
        }
    }
    delete[] buffer;
    InternetCloseHandle(hOpenUrl);
    InternetCloseHandle(hSession);

    return data;
}

string InitializeLightFXExtender() {
    wstring pathw;
    string path;
    wchar_t* appDataFolder = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataFolder))) {
        pathw = wstring(appDataFolder) + L"\\LightFX Extender";
        if (!CreateDirectoryW(pathw.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
            // Failed to create directory
            throw exception("Failed to create the AppData storage directory.");
        }
        pathw += L"\\";
        path = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(pathw);

        CoTaskMemFree(appDataFolder);
    } else {
        // Failed to get our storage folder
        CoTaskMemFree(appDataFolder);
        throw exception("Failed to get the AppData storage directory location.");
    }

    // Get the release information from the GitHub API
    vector<char> releaseData;
    try {
        releaseData = DownloadFromUrl(GITHUB_RELEASEURL_API);
    } catch (int e) {
        switch (e) {
        case DLERROR_OPENSESSION:
            throw exception("Failed to open the internet connection to GitHub in order to get the latest release information.");
        case DLERROR_OPENURL:
            throw exception("Failed to connect to GitHub in order to get the latest release information.");
        case DLERROR_READURL:
            throw exception("Failed to read the latest release information on GitHub.");
        default:
            throw exception("Unknown error while downloading the latest release information from GitHub.");
        }
    }

    // Parse the release information
    string version;
    string downloadUrl;
    string releaseString(releaseData.begin(), releaseData.end());
    Document releaseJson;
    releaseJson.Parse<0>(releaseString.c_str());
    if (releaseJson.IsObject()) {
        // Get version
        if (releaseJson.HasMember("tag_name") && releaseJson["tag_name"].IsString()) {
            version = releaseJson["tag_name"].GetString();
        }

        // Get the asset download URL
        if (releaseJson.HasMember("assets") && releaseJson["assets"].IsArray()) {
            auto& assets = releaseJson["assets"];
            regex re("LightFX-Extender-full_v(\\d+\\.\\d+\\.\\d+\\.\\d+)\\.zip");
            smatch match;
            for (SizeType i = 0; i < assets.Size(); ++i) {
                if (assets[i].HasMember("name") && assets[i]["name"].IsString()) {
                    string assetName = assets[i]["name"].GetString();
                    if (regex_search(assetName, match, re) && match.size() > 1) {
                        if (assets[i].HasMember("browser_download_url") && assets[i]["browser_download_url"].IsString()) {
                            downloadUrl = assets[i]["browser_download_url"].GetString();
                            break;
                        }
                    }
                }
            }
        }
    }

    if (version.empty()) {
        // Failed to get the latest version
        throw exception("There is no latest release of LightFX Extender.");
    } else if (downloadUrl.empty()) {
        // Failed to get the GitHub asset download
        throw exception(("The latest version (" + version + ") of LightFX Extender has no downloads available.").c_str());
    }

    // Proceed to download the release
    vector<char> releaseZip;
    try {
        releaseZip = DownloadFromUrl(wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(downloadUrl));
    } catch (int e) {
        switch (e) {
        case DLERROR_OPENSESSION:
            throw exception("Failed to open the internet connection to GitHub in order to download the latest release.");
        case DLERROR_OPENURL:
            throw exception("Failed to connect to GitHub in order to download the latest release.");
        case DLERROR_READURL:
            throw exception("Failed to download the latest release from GitHub.");
        default:
            throw exception("Unknown error while downloading the latest release from GitHub.");
        }
    }

    // Extract the files from the archive
    mz_zip_archive archive;
    memset(&archive, 0, sizeof(archive));
    if (!mz_zip_reader_init_mem(&archive, &releaseZip[0], releaseZip.size(), 0)) {
        // Failed to open the archive
        throw exception("Failed to open the downloaded archive.");
    }

    mz_zip_archive_file_stat filestat;
    if (!mz_zip_reader_file_stat(&archive, 0, &filestat)) {
        // Failed to read the archive
        mz_zip_reader_end(&archive);
        throw exception("Failed to read the downloaded archive.");
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
                // Failed to create directory
                throw exception("Failed to create the x86 directory in the AppData storage directory.");
            }
            filename = path + filename;
        } else if (filename.compare(0, 4, "x64/") == 0) {
            // Additional x64 files, extract to relative folder
            if (!CreateDirectoryW((pathw + L"\\x64").c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
                // Failed to create directory
                throw exception("Failed to create the x64 directory in the AppData storage directory.");
            }
            filename = path + filename;
        } else {
            // Other file, skip
            continue;
        }

        if (!mz_zip_reader_extract_to_file(&archive, i, filename.c_str(), 0)) {
            // Failed to extract file
            mz_zip_reader_end(&archive);
            throw exception(("Failed to extract a file to the following location: " + filename).c_str());
        }
    }

    return version;
}

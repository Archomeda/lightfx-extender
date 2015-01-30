#include "UpdateManager.h"

// Windows includes
#include "Common/Windows.h"
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

// 3rd party includes
#include "rapidjson/document.h"

// Project includes
#include "Utils/Log.h"


using namespace std;
using namespace lightfx::utils;
using namespace rapidjson;

namespace lightfx {

    Version UpdateManager::GetLiveVersion() {
        HINTERNET hSession = InternetOpenW(L"LightFX Extender Update Checker", 0, NULL, NULL, 0);
        if (hSession == NULL) {
            Log(L"Failed to open session");
            return Version();
        }

        HINTERNET hOpenUrl = InternetOpenUrlW(hSession, this->GitHub_ApiTagsUrl.c_str(), NULL, 0, 1, 1);
        if (hOpenUrl == NULL) {
            Log(L"Failed to open URL");
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
                Log(L"Failed to read web page");
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

}

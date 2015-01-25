#include "FileIO.h"

// Windows includes
#include <Windows.h>
#include <ShlObj.h>


using namespace std;

namespace lightfx {
    namespace utils {

        bool DirExists(const wstring& path) {
            DWORD fileType = GetFileAttributesW(path.c_str());
            if (fileType == INVALID_FILE_ATTRIBUTES) {
                return false;
            } else if (fileType & FILE_ATTRIBUTE_DIRECTORY) {
                return true;
            }
            return false;
        }

        bool GetRoamingAppDataFolder(wstring& path) {
            path = L"";
            wchar_t* appData = nullptr;
            if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appData))) {
                path = wstring(appData);
            }
            CoTaskMemFree(appData);

            return !path.empty();
        }

    }
}
#pragma once

// Standard includes
#include <string>


#define APPDATA_FOLDER L"LightFX Extender"

namespace lightfx {
    namespace utils {

        bool DirExists(const std::wstring& path);
        bool GetRoamingAppDataFolder(std::wstring& path);

    }
}

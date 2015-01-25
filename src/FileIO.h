#pragma once
#include <string>

#define APPDATA_FOLDER L"LightFX Extender"

namespace lightfx {
    namespace fileio {

        bool DirExists(const std::wstring& path);
        bool GetRoamingAppDataFolder(std::wstring& path);

    }
}

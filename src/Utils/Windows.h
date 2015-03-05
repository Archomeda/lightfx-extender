#pragma once

// Standard includes
#include <string>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>


namespace lightfx {
    namespace utils {

        extern "C" IMAGE_DOS_HEADER __ImageBase;
        HMODULE GetCurrentModule();
        std::wstring GetProcessName();
        std::wstring GetProcessName(std::wstring* drive, std::wstring* dir, std::wstring* fname, std::wstring* ext);
        std::wstring GetDllName();
        std::wstring GetDllName(std::wstring* drive, std::wstring* dir, std::wstring* fname, std::wstring* ext);

    }
}

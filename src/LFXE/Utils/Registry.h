#pragma once

// Standard includes
#include <string>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#pragma comment(lib, "Advapi32.lib")

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace utils {

        LFXE_API std::wstring GetRegKeyString(const HKEY hKey, const std::wstring& subKey, const std::wstring& value, const std::wstring& defaultString, REGSAM samDesired = NULL);

    }
}

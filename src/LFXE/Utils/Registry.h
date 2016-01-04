#pragma once

// Standard includes
#include <string>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#pragma comment(lib, "Advapi32.lib")

// API exports
#include "../Common/ApiExports.h"

#define REGKEY_LFXE L"SOFTWARE\\LightFX Extender"
#define REGKEY_UNINSTALL L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"


namespace lightfx {
    namespace utils {

        LFXE_API unsigned long GetRegKeyDword(const HKEY hKEy, const std::wstring& subKey, const std::wstring& value, const unsigned long defaultDword, REGSAM samDesired = NULL);
        LFXE_API bool SetRegKeyDword(const HKEY hKEy, const std::wstring& subKey, const std::wstring& value, const unsigned long data, REGSAM samDesired = NULL);

        LFXE_API std::wstring GetRegKeyString(const HKEY hKey, const std::wstring& subKey, const std::wstring& value, const std::wstring& defaultString, REGSAM samDesired = NULL);
        LFXE_API bool SetRegKeyString(const HKEY hKey, const std::wstring& subKey, const std::wstring& value, const std::wstring& data, REGSAM samDesired = NULL);

    }
}

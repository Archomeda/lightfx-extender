#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Registry.h"


using namespace std;

namespace lightfx {
    namespace utils {

        LFXE_API std::wstring GetRegKeyString(const HKEY hKey, const wstring& subKey, const wstring& value, const wstring& defaultString, REGSAM samDesired) {
            HKEY hKey2;
            if (RegOpenKeyExW(hKey, subKey.c_str(), NULL, samDesired | KEY_READ, &hKey2) != ERROR_SUCCESS) {
                return defaultString;
            }

            WCHAR szBuffer[1024];
            DWORD bufferSize = sizeof(szBuffer);
            if (RegQueryValueExW(hKey2, value.c_str(), NULL, NULL, (LPBYTE)szBuffer, &bufferSize) != ERROR_SUCCESS) {
                RegCloseKey(hKey2);
                return defaultString;
            }

            RegCloseKey(hKey2);
            return wstring(szBuffer);
        }

    }
}
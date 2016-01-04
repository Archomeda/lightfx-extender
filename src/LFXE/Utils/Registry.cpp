#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Registry.h"


using namespace std;

namespace lightfx {
    namespace utils {

        LFXE_API unsigned long GetRegKeyDword(const HKEY hKey, const wstring& subKey, const wstring& value, const unsigned long defaultDword, REGSAM samDesired) {
            HKEY hKey2;
            if (RegOpenKeyExW(hKey, subKey.c_str(), NULL, samDesired | KEY_READ, &hKey2) != ERROR_SUCCESS) {
                return defaultDword;
            }

            DWORD buffer(0);
            DWORD bufferSize = sizeof(buffer);
            if (RegQueryValueExW(hKey2, value.c_str(), NULL, NULL, (LPBYTE)buffer, &bufferSize) != ERROR_SUCCESS) {
                RegCloseKey(hKey2);
                return defaultDword;
            }

            RegCloseKey(hKey2);
            return (unsigned long)buffer;
        }

        LFXE_API bool SetRegKeyDword(const HKEY hKey, const wstring& subKey, const wstring& value, const unsigned long data, REGSAM samDesired) {
            HKEY hKey2;
            if (RegOpenKeyExW(hKey, subKey.c_str(), NULL, samDesired | KEY_READ | KEY_WRITE, &hKey2) != ERROR_SUCCESS) {
                return false;
            }

            if (RegSetValueExW(hKey2, value.c_str(), NULL, REG_DWORD, (LPBYTE)&data, sizeof(data)) != ERROR_SUCCESS) {
                RegCloseKey(hKey2);
                return false;
            }

            RegCloseKey(hKey2);
            return true;
        }

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

        LFXE_API bool SetRegKeyString(const HKEY hKey, const wstring& subKey, const wstring& value, const wstring& data, REGSAM samDesired) {
            HKEY hKey2;
            if (RegOpenKeyExW(hKey, subKey.c_str(), NULL, samDesired | KEY_READ | KEY_WRITE, &hKey2) != ERROR_SUCCESS) {
                return false;
            }

            if (RegSetValueExW(hKey2, value.c_str(), NULL, REG_SZ, (LPBYTE)data.c_str(), (DWORD)(data.size() * sizeof(wchar_t))) != ERROR_SUCCESS) {
                RegCloseKey(hKey2);
                return false;
            }

            RegCloseKey(hKey2);
            return true;
        }

    }
}
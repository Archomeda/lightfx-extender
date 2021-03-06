#pragma once

// Standard includes
#include <string>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace utils {

        LFXE_API inline std::string wstring_to_string(const std::wstring& wstr) {
            if (wstr.empty()) {
                return std::string();
            }
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
        }

        LFXE_API inline std::wstring string_to_wstring(const std::string& str) {
            if (str.empty()) {
                return std::wstring();
            }
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
        }

        LFXE_API std::vector<std::string> split(const std::string& str, const char delim);
        LFXE_API std::vector<std::string> split(const std::string& str, const char delim, const size_t amount);

        LFXE_API std::vector<std::wstring> split(const std::wstring& str, const wchar_t delim);
        LFXE_API std::vector<std::wstring> split(const std::wstring& str, const wchar_t delim, const size_t amount);

    }
}

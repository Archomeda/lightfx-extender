#pragma once

// Standard includes
#include <string>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>


namespace lightfx {
    namespace utils {

        inline std::string wstring_to_string(const std::wstring& wstr) {
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
        }

        inline std::wstring string_to_wstring(const std::string& str) {
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
        }

        std::vector<std::string> split(const std::string& str, char delim);

        std::vector<std::wstring> split(const std::wstring& str, wchar_t delim);

    }
}
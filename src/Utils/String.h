#pragma once

// Standard includes
#include <string>
#include <locale>
#include <codecvt>


namespace lightfx {
    namespace utils {

        inline std::string wstring_to_string(std::wstring wstr) {
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
        }

        inline std::wstring string_to_wstring(std::string str) {
            return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
        }

    }
}
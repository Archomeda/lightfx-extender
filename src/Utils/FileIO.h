#pragma once

// Standard includes
#include <string>

// API exports
#include "../Common/ApiExports.h"



namespace lightfx {
    namespace utils {

        LFXE_API bool DirExists(const std::wstring& path);
        LFXE_API bool FileExists(const std::wstring& path);
        LFXE_API std::wstring GetDataStorageFolder();

    }
}

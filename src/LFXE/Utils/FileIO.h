#pragma once

// Standard includes
#include <string>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace utils {

        LFXE_API bool DirExists(const std::wstring& path);
        LFXE_API bool FileExists(const std::wstring& path);
        LFXE_API bool CreateDirIfNotExists(const std::wstring& path);
        LFXE_API bool RenameFile(const std::wstring& source, const std::wstring& target);
        LFXE_API bool RemoveFile(const std::wstring& path);

        LFXE_API std::wstring GetDataStorageFolder();
        LFXE_API std::wstring GetProgramFolder();
        LFXE_API std::wstring GetSystemFolder();

    }
}

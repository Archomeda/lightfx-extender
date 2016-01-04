#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "FileIO.h"

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#include <ShlObj.h>

// Project includes
#include "Registry.h"
#include "../Exceptions/AccessDeniedException.h"

#define STORAGEFOLDER L"LightFX Extender"


using namespace std;
using namespace lightfx::exceptions;

namespace lightfx {
    namespace utils {

        LFXE_API bool DirExists(const wstring& path) {
            DWORD fileType = GetFileAttributesW(path.c_str());
            if (fileType == INVALID_FILE_ATTRIBUTES) {
                return false;
            } else if (fileType & FILE_ATTRIBUTE_DIRECTORY) {
                return true;
            }
            return false;
        }

        LFXE_API bool FileExists(const wstring& path) {
            DWORD fileType = GetFileAttributesW(path.c_str());
            return fileType != INVALID_FILE_ATTRIBUTES;
        }

        LFXE_API bool CreateDirIfNotExists(const wstring& path) {
            if (!CreateDirectoryW(path.c_str(), NULL)) {
                DWORD error = GetLastError();
                if (error == ERROR_ACCESS_DENIED) {
                    throw AccessDeniedException();
                } else if (error == ERROR_PATH_NOT_FOUND) {
                    // Assume that a parent directory doesn't exist, so let's try to create that one
                    size_t pos = path.find_last_of(L"/\\");
                    if (pos == wstring::npos) {
                        return false;
                    }
                    if (CreateDirIfNotExists(path.substr(0, pos))) {
                        if (!CreateDirectoryW(path.c_str(), NULL)) {
                            error = GetLastError();
                            if (error == ERROR_ACCESS_DENIED) {
                                throw AccessDeniedException();
                            } else {
                                return false;
                            }
                        }
                    }
                }
            }
            return true;
        }

        LFXE_API bool RenameFile(const wstring& source, const wstring& target) {
            if (FileExists(source)) {
                if (!MoveFileW(source.c_str(), target.c_str())) {
                    if (GetLastError() == ERROR_ACCESS_DENIED) {
                        throw AccessDeniedException();
                    } else {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        LFXE_API bool RemoveFile(const wstring& path) {
            if (FileExists(path)) {
                if (!DeleteFileW(path.c_str())) {
                    if (GetLastError() == ERROR_ACCESS_DENIED) {
                        throw AccessDeniedException();
                    } else {
                        return false;
                    }
                }
            }
            return true;
        }

        wstring GetKnownFolder(const GUID folderId) {
            wstring path = L"";
            wchar_t* appData = nullptr;
            if (SUCCEEDED(SHGetKnownFolderPath(folderId, 0, NULL, &appData))) {
                path = wstring(appData);
            }
            CoTaskMemFree(appData);

            return path;
        }

        LFXE_API wstring GetDataStorageFolder() {
            wstring path = GetKnownFolder(FOLDERID_RoamingAppData);
            if (!path.empty()) {
                return path + L"\\" + STORAGEFOLDER;
            } else {
                return L"";
            }
        }

        LFXE_API wstring GetProgramFolder() {
            return GetRegKeyString(HKEY_LOCAL_MACHINE, REGKEY_LFXE, L"InstallPath", GetDataStorageFolder(), KEY_WOW64_64KEY);
        }

        LFXE_API wstring GetSystemFolder() {
            return GetKnownFolder(FOLDERID_System);
        }

    }
}
#include "Windows.h"


using namespace std;

namespace lightfx {
    namespace utils {

        HMODULE GetCurrentModule() {
            return reinterpret_cast<HMODULE>(&__ImageBase);
        }

        wstring GetModuleName(HINSTANCE hInstance, wstring* drive, wstring* dir, wstring* fname, wstring* ext) {
            wchar_t szFileName[MAX_PATH];
            GetModuleFileNameW(hInstance, szFileName, _countof(szFileName));
            wchar_t cdrive[_MAX_DRIVE];
            wchar_t cdir[_MAX_DIR];
            wchar_t cfname[_MAX_FNAME];
            wchar_t cext[_MAX_EXT];
            _wsplitpath_s(szFileName, cdrive, _countof(cdrive), cdir, _countof(cdir), cfname, _countof(cfname), cext, _countof(cext));

            if (drive != nullptr) {
                *drive = wstring(cdrive);
            }
            if (dir != nullptr) {
                *dir = wstring(cdir);
            }
            if (fname != nullptr) {
                *fname = wstring(cfname);
            }
            if (ext != nullptr) {
                *ext = wstring(cext);
            }

            return wstring(szFileName);
        }

        wstring GetProcessName() {
            return GetProcessName(nullptr, nullptr, nullptr, nullptr);
        }

        wstring GetProcessName(wstring* drive, wstring* dir, wstring* fname, wstring* ext) {
            return GetModuleName(NULL, drive, dir, fname, ext);
        }

        wstring GetDllName() {
            return GetDllName(nullptr, nullptr, nullptr, nullptr);
        }

        wstring GetDllName(wstring* drive, wstring* dir, wstring* fname, wstring* ext) {
            return GetModuleName(GetCurrentModule(), drive, dir, fname, ext);
        }

    }
}

#include "Windows.h"


using namespace std;

namespace lightfx {
    namespace utils {

        HMODULE GetCurrentModule() {
            return reinterpret_cast<HMODULE>(&__ImageBase);
        }

        wstring GetProcessName() {
            return GetProcessName(nullptr, nullptr, nullptr, nullptr);
        }

        wstring GetProcessName(wstring* drive, wstring* dir, wstring* fname, wstring* ext) {
            wchar_t szFileName[MAX_PATH];
            GetModuleFileNameW(NULL, szFileName, MAX_PATH);
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

    }
}

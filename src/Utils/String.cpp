#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "String.h"


using namespace std;

namespace lightfx {
    namespace utils {

        LFXE_API vector<string> split(const string& str, const char delim) {
            stringstream ss(str);
            string item;
            vector<string> elems = {};
            while (getline(ss, item, delim)) {
                elems.push_back(item);
            }
            return elems;
        }

        LFXE_API vector<wstring> split(const wstring& str, const wchar_t delim) {
            wstringstream ss(str);
            wstring item;
            vector<wstring> elems = {};
            while (getline(ss, item, delim)) {
                elems.push_back(item);
            }
            return elems;
        }

    }
}

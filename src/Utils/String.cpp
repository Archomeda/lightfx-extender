#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "String.h"


using namespace std;

namespace lightfx {
    namespace utils {

        LFXE_API vector<string> split(const string& str, const char delim) {
            return split(str, delim, 0);
        }

        LFXE_API vector<string> split(const string& str, const char delim, const size_t amount) {
            stringstream ss(str);
            string item;
            vector<string> elems = {};
            while (amount == 0 || elems.size() + 1 < amount) {
                if (getline(ss, item, delim)) {
                    elems.push_back(item);
                } else {
                    break;
                }
            }
            if (getline(ss, item)) {
                elems.push_back(item);
            }
            return elems;
        }

        LFXE_API vector<wstring> split(const wstring& str, const wchar_t delim) {
            return split(str, delim, 0);
        }

        LFXE_API vector<wstring> split(const wstring& str, const wchar_t delim, const size_t amount) {
            wstringstream ss(str);
            wstring item;
            vector<wstring> elems = {};
            while (amount == 0 || elems.size() + 1 < amount) {
                if (getline(ss, item, delim)) {
                    elems.push_back(item);
                } else {
                    break;
                }
            }
            if (getline(ss, item)) {
                elems.push_back(item);
            }
            return elems;
        }

    }
}

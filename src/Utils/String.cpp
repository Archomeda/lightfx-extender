#include "String.h"


using namespace std;

namespace lightfx {
    namespace utils {

        vector<string> split(const string& str, char delim) {
            stringstream ss(str);
            string item;
            vector<string> elems = {};
            while (getline(ss, item, delim)) {
                elems.push_back(item);
            }
            return elems;
        }

        vector<wstring> split(const wstring& str, wchar_t delim) {
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
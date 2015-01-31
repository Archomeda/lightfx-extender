#define LIGHTFX_EXPORTS
#include "Version.h"

// Standard includes
#include <vector>

// Project includes
#include "Utils/String.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {

    Version::Version(const string& str) {
        vector<string> splitted = split(str, '.');
        if (splitted.size() == 0) {
            Version::Version();
        }

        try {
            this->majorVer = splitted.size() >= 1 ? stoi(splitted.at(0)) : 0;
        } catch (const invalid_argument&) {
            this->majorVer = 0;
        }
        try {
            this->minorVer = splitted.size() >= 2 ? stoi(splitted.at(1)) : 0;
        } catch (const invalid_argument&) {
            this->minorVer = 0;
        }
        try {
            this->buildVer = splitted.size() >= 3 ? stoi(splitted.at(2)) : 0;
        } catch (const invalid_argument&) {
            this->buildVer = 0;
        }
    }

    Version::Version(const wstring& str) {
        vector<wstring> splitted = split(str, '.');
        if (splitted.size() == 0) {
            Version::Version();
        }

        try {
            this->majorVer = splitted.size() >= 1 ? stoi(splitted.at(0)) : 0;
        } catch (const invalid_argument&) {
            this->majorVer = 0;
        }
        try {
            this->minorVer = splitted.size() >= 2 ? stoi(splitted.at(1)) : 0;
        } catch (const invalid_argument&) {
            this->minorVer = 0;
        }
        try {
            this->buildVer = splitted.size() >= 3 ? stoi(splitted.at(2)) : 0;
        } catch (const invalid_argument&) {
            this->buildVer = 0;
        }
    }

}

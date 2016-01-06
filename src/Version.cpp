#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Version.h"

// Project includes
#include "Utils/String.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {

    LFXE_API wstring Version::ToString() {
        wstring version = to_wstring(this->GetMajor()) + L"." + to_wstring(this->GetMinor()) + L"." + to_wstring(this->GetSubMinor()) + L"." + to_wstring(this->GetBuild());
        if (this->GetDescription() != L"") {
            version += L"-" + this->GetDescription();
        }
        return version;
    }

    LFXE_API void Version::SetFromString(const string& str) {
        this->SetFromString(string_to_wstring(str));
    }

    LFXE_API void Version::SetFromString(const wstring& str) {
        vector<wstring> splitted1 = split(str, '-', 2);
        if (splitted1.size() > 0) {
            vector<wstring> splitted2 = split(splitted1[0], '.');
            try {
                this->majorVer = splitted2.size() >= 1 ? stoi(splitted2[0]) : 0;
            } catch (const invalid_argument&) {
                this->majorVer = 0;
            }
            try {
                this->minorVer = splitted2.size() >= 2 ? stoi(splitted2[1]) : 0;
            } catch (const invalid_argument&) {
                this->minorVer = 0;
            }
            try {
                this->subMinorVer = splitted2.size() >= 3 ? stoi(splitted2[2]) : 0;
            } catch (const invalid_argument&) {
                this->subMinorVer = 0;
            }
            try {
                this->buildVer = splitted2.size() >= 4 ? stoi(splitted2[3]) : 0;
            } catch (const invalid_argument&) {
                this->buildVer = 0;
            }

            if (splitted1.size() >= 2) {
                this->description = splitted1[1];
            }
        } else {
            this->majorVer = 0;
            this->minorVer = 0;
            this->subMinorVer = 0;
            this->buildVer = 0;
        }
    }

}
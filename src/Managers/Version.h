#pragma once

// Standard includes
#include <string>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace managers {

        class LFXE_API Version {

        public:
            Version() : Version(0, 0, 0) {}
            Version(const unsigned int major, const unsigned int minor, const unsigned int build) : majorVer(major), minorVer(minor), buildVer(build) {}

            static Version FromString(const std::string& str) {
                return Version(str);
            }

            static Version FromString(const std::wstring& str) {
                return Version(str);
            }

            unsigned int GetMajor() { return this->majorVer; }
            unsigned int GetMinor() { return this->minorVer; }
            unsigned int GetBuild() { return this->buildVer; }

            std::wstring ToString();

            friend inline bool operator==(const Version& a, const Version& b) {
                return a.majorVer == b.majorVer && a.minorVer == b.minorVer && a.buildVer == b.buildVer;
            }
            friend inline bool operator!=(const Version& a, const Version& b) { return !operator==(a, b); }
            friend inline bool operator<(const Version& a, const Version& b) {
                return a.majorVer < b.majorVer ||
                    (a.majorVer == b.majorVer && a.minorVer < b.minorVer) ||
                    (a.majorVer == b.majorVer && a.minorVer == b.minorVer && a.buildVer < b.buildVer);
            }
            friend inline bool operator>(const Version& a, const Version& b) { return operator<(b, a); }
            friend inline bool operator<=(const Version& a, const Version& b) { return !operator<(b, a); }
            friend inline bool operator>=(const Version& a, const Version& b) { return !operator<(a, b); }

        protected:
            Version(const std::string& str);
            Version(const std::wstring& str);

        private:
            unsigned int majorVer;
            unsigned int minorVer;
            unsigned int buildVer;

        };

    }

}

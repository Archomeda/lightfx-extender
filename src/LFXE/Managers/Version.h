#pragma once

// Standard includes
#include <string>

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace managers {

        class LFXE_API Version {

        public:
            Version() : Version(0, 0, 0, 0) {}
            Version(const unsigned int major, const unsigned int minor, const unsigned int subMinor, const unsigned int build)
                : majorVer(major), minorVer(minor), subMinorVer(subMinor), buildVer(build) {}
            Version(const unsigned int major, const unsigned int minor, const unsigned int subMinor, const unsigned int build, const std::wstring& description)
                : Version(major, minor, subMinor, build) {
                this->description = description;
            }

            static Version FromString(const std::string& str) {
                return Version(str);
            }

            static Version FromString(const std::wstring& str) {
                return Version(str);
            }

            unsigned int GetMajor() const { return this->majorVer; }
            unsigned int GetMinor() const { return this->minorVer; }
            unsigned int GetSubMinor() const { return this->subMinorVer; }
            unsigned int GetBuild() const { return this->buildVer; }
            std::wstring GetDescription() const { return this->description; }

            std::wstring ToString() const;

            friend inline bool operator==(const Version& a, const Version& b) {
                return a.majorVer == b.majorVer && a.minorVer == b.minorVer && a.subMinorVer == b.subMinorVer && a.buildVer == b.buildVer;
            }
            friend inline bool operator!=(const Version& a, const Version& b) { return !operator==(a, b); }
            friend inline bool operator<(const Version& a, const Version& b) {
                return a.majorVer < b.majorVer ||
                    (a.majorVer == b.majorVer && a.minorVer < b.minorVer) ||
                    (a.majorVer == b.majorVer && a.minorVer == b.minorVer && a.subMinorVer < b.subMinorVer) ||
                    (a.majorVer == b.majorVer && a.minorVer == b.minorVer && a.subMinorVer == b.subMinorVer && a.buildVer < b.buildVer);
            }
            friend inline bool operator>(const Version& a, const Version& b) { return operator<(b, a); }
            friend inline bool operator<=(const Version& a, const Version& b) { return !operator<(b, a); }
            friend inline bool operator>=(const Version& a, const Version& b) { return !operator<(a, b); }

        protected:
            Version(const std::string& str) {
                this->SetFromString(str);
            }
            Version(const std::wstring& str) {
                this->SetFromString(str);
            }

            void SetFromString(const std::string& str);
            void SetFromString(const std::wstring& str);

        private:
            unsigned int majorVer;
            unsigned int minorVer;
            unsigned int subMinorVer;
            unsigned int buildVer;
            std::wstring description;

        };

    }

}

#pragma warning(pop)

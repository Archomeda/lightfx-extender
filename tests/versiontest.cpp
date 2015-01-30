#include "CppUnitTest.h"
#include "../src/Version.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lightfx;

namespace lightfx_tests {
    TEST_CLASS(VersionTest) {
    public:

        TEST_METHOD(ContructorDefault) {
            Version ver = Version();
            Assert::AreEqual<unsigned int>(0, ver.GetMajor(), L"Major");
            Assert::AreEqual<unsigned int>(0, ver.GetMinor(), L"Minor");
            Assert::AreEqual<unsigned int>(0, ver.GetBuild(), L"Build");
        }

        TEST_METHOD(ContructorEx) {
            Version ver(1, 2, 3);
            Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
            Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
            Assert::AreEqual<unsigned int>(3, ver.GetBuild(), L"Build");
        }

        TEST_METHOD(FromStringEmpty) {
            Version ver = Version::FromString("");
            Assert::AreEqual<unsigned int>(0, ver.GetMajor(), L"Major");
            Assert::AreEqual<unsigned int>(0, ver.GetMinor(), L"Minor");
            Assert::AreEqual<unsigned int>(0, ver.GetBuild(), L"Build");
        }

        TEST_METHOD(FromStringFullFormat) {
            Version ver = Version::FromString("1.2.3");
            Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
            Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
            Assert::AreEqual<unsigned int>(3, ver.GetBuild(), L"Build");
        }

        TEST_METHOD(FromStringPartialFormat) {
            Version ver = Version::FromString("1.2");
            Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
            Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
            Assert::AreEqual<unsigned int>(0, ver.GetBuild(), L"Build");
        }

        TEST_METHOD(FromStringMisformedFormat1) {
            Version ver = Version::FromString("1.2.3aa");
            Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
            Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
            Assert::AreEqual<unsigned int>(3, ver.GetBuild(), L"Build");
        }

        TEST_METHOD(FromStringMisformedFormat2) {
            Version ver = Version::FromString("1.2.aa");
            Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
            Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
            Assert::AreEqual<unsigned int>(0, ver.GetBuild(), L"Build");
        }
    };
}
#include "CppUnitTest.h"
#include "../../src/Managers/Version.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lightfx::managers;

namespace lightfx_tests {
    namespace managers {

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

    TEST_METHOD(FromWStringEmpty) {
        Version ver = Version::FromString(L"");
        Assert::AreEqual<unsigned int>(0, ver.GetMajor(), L"Major");
        Assert::AreEqual<unsigned int>(0, ver.GetMinor(), L"Minor");
        Assert::AreEqual<unsigned int>(0, ver.GetBuild(), L"Build");
    }

    TEST_METHOD(FromWStringFullFormat) {
        Version ver = Version::FromString(L"1.2.3");
        Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
        Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
        Assert::AreEqual<unsigned int>(3, ver.GetBuild(), L"Build");
    }

    TEST_METHOD(FromWStringPartialFormat) {
        Version ver = Version::FromString(L"1.2");
        Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
        Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
        Assert::AreEqual<unsigned int>(0, ver.GetBuild(), L"Build");
    }

    TEST_METHOD(FromWStringMisformedFormat1) {
        Version ver = Version::FromString(L"1.2.3aa");
        Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
        Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
        Assert::AreEqual<unsigned int>(3, ver.GetBuild(), L"Build");
    }

    TEST_METHOD(FromWStringMisformedFormat2) {
        Version ver = Version::FromString(L"1.2.aa");
        Assert::AreEqual<unsigned int>(1, ver.GetMajor(), L"Major");
        Assert::AreEqual<unsigned int>(2, ver.GetMinor(), L"Minor");
        Assert::AreEqual<unsigned int>(0, ver.GetBuild(), L"Build");
    }

    TEST_METHOD(OperatorEquals) {
        Version verA = Version(1, 2, 3);
        Version verB = Version(1, 2, 3);
        Version verC = Version(2, 3, 4);

        Assert::IsTrue(verA == verB, L"verA == verB");
        Assert::IsFalse(verA == verC, L"verA == verC");
    }

    TEST_METHOD(OperatorNotEquals) {
        Version verA = Version(1, 2, 3);
        Version verB = Version(2, 3, 4);
        Version verC = Version(1, 2, 3);

        Assert::IsTrue(verA != verB, L"verA != verB");
        Assert::IsFalse(verA != verC, L"verA != verC");
    }

    TEST_METHOD(OperatorSmallerThan) {
        Version verA = Version(1, 2, 3);
        Version verB = Version(1, 2, 4);
        Version verC = Version(1, 3, 1);
        Version verD = Version(2, 0, 0);
        Version verE = Version(1, 2, 3);
        Version verF = Version(1, 2, 2);
        Version verG = Version(1, 1, 4);
        Version verH = Version(0, 2, 4);

        Assert::IsTrue(verA < verB, L"verA < verB");
        Assert::IsTrue(verA < verC, L"verA < verC");
        Assert::IsTrue(verA < verD, L"verA < verD");
        Assert::IsFalse(verA < verE, L"verA < verE");
        Assert::IsFalse(verA < verF, L"verA < verF");
        Assert::IsFalse(verA < verG, L"verA < verG");
        Assert::IsFalse(verA < verH, L"verA < verH");
    }

    TEST_METHOD(OperatorGreaterThan) {
        Version verA = Version(1, 2, 3);
        Version verB = Version(1, 2, 2);
        Version verC = Version(1, 1, 4);
        Version verD = Version(0, 2, 4);
        Version verE = Version(1, 2, 3);
        Version verF = Version(1, 2, 4);
        Version verG = Version(1, 3, 1);
        Version verH = Version(2, 0, 0);

        Assert::IsTrue(verA > verB, L"verA > verB");
        Assert::IsTrue(verA > verC, L"verA > verC");
        Assert::IsTrue(verA > verD, L"verA > verD");
        Assert::IsFalse(verA > verE, L"verA > verE");
        Assert::IsFalse(verA > verF, L"verA > verF");
        Assert::IsFalse(verA > verG, L"verA > verG");
        Assert::IsFalse(verA > verH, L"verA > verH");
    }

    TEST_METHOD(OperatorSmallerThanOrEqual) {
        Version verA = Version(1, 2, 3);
        Version verB = Version(1, 2, 4);
        Version verC = Version(1, 3, 1);
        Version verD = Version(2, 0, 0);
        Version verE = Version(1, 2, 3);
        Version verF = Version(1, 2, 2);
        Version verG = Version(1, 1, 4);
        Version verH = Version(0, 2, 4);

        Assert::IsTrue(verA <= verB, L"verA <= verB");
        Assert::IsTrue(verA <= verC, L"verA <= verC");
        Assert::IsTrue(verA <= verD, L"verA <= verD");
        Assert::IsTrue(verA <= verE, L"verA <= verE");
        Assert::IsFalse(verA <= verF, L"verA <= verF");
        Assert::IsFalse(verA <= verG, L"verA <= verG");
        Assert::IsFalse(verA <= verH, L"verA <= verH");
    }

    TEST_METHOD(OperatorGreaterThanOrEqual) {
        Version verA = Version(1, 2, 3);
        Version verB = Version(1, 2, 2);
        Version verC = Version(1, 1, 4);
        Version verD = Version(0, 2, 4);
        Version verE = Version(1, 2, 3);
        Version verF = Version(1, 2, 4);
        Version verG = Version(1, 3, 1);
        Version verH = Version(2, 0, 0);

        Assert::IsTrue(verA >= verB, L"verA >= verB");
        Assert::IsTrue(verA >= verC, L"verA >= verC");
        Assert::IsTrue(verA >= verD, L"verA >= verD");
        Assert::IsTrue(verA >= verE, L"verA >= verE");
        Assert::IsFalse(verA >= verF, L"verA >= verF");
        Assert::IsFalse(verA >= verG, L"verA >= verG");
        Assert::IsFalse(verA >= verH, L"verA >= verH");
    }

        };
    }
}

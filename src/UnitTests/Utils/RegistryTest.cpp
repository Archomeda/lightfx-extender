#include "../../LFXE/Utils/Registry.h"
#include "CppUnitTest.h"

// Windows includes
#include <ShlObj.h>


using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lightfx::utils;

namespace lightfx_tests {
    namespace utils {

        TEST_CLASS(RegistryTest) {
public:

    TEST_METHOD(GetRegKeyDwordWindowsDoesExist) {
        // This assumes that the system runs Windows where we can query the install date
        unsigned long defaultDword = 1;
        unsigned long result = GetRegKeyDword(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"InstallDate", defaultDword);
        Assert::AreNotEqual(defaultDword, result);
    }

    TEST_METHOD(GetRegKeyDwordWindowsDoesNotExist) {
        unsigned long defaultDword = 1;
        unsigned long result = GetRegKeyDword(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion2", L"InstallDate", defaultDword);
        Assert::AreEqual(defaultDword, result);
    }

    TEST_METHOD(GetRegKeyStringWindowsDoesExist) {
        // This assumes that the system runs Windows where we can query the current Windows version
        wstring defaultString = L"SomethingDefault";
        wstring result = GetRegKeyString(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentVersion", defaultString);
        Assert::AreNotEqual(defaultString, result);
    }

    TEST_METHOD(GetRegKeyStringWindowsDoesNotExist) {
        wstring defaultString = L"SomethingDefault";
        wstring result = GetRegKeyString(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion2", L"CurrentVersion", defaultString);
        Assert::AreEqual(defaultString, result);
    }

        };
    }
}

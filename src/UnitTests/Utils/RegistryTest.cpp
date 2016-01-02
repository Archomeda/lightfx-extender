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

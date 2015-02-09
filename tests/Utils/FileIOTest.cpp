#include "CppUnitTest.h"
#include "../../src/Utils/FileIO.h"

// Windows includes
#include <ShlObj.h>


using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lightfx::utils;

namespace utils {
    TEST_CLASS(FileIOTest) {
public:

    TEST_METHOD(DirectoryDoesNotExist) {
        wstring dir = L"DirectoryThatDoesNotExist";
        Assert::IsFalse(DirExists(dir));
    }

    TEST_METHOD(DirectoryDoesExist) {
        wstring dir = L"DirectoryThatDoesExist";
        CreateDirectoryW(dir.c_str(), NULL);

        bool exists = DirExists(dir);
        RemoveDirectoryW(dir.c_str());
        Assert::IsTrue(exists);
    }

    TEST_METHOD(StorageDirectoryNotEmpty) {
        Assert::IsFalse(GetDataStorageFolder() == L"");
    }

    };
}

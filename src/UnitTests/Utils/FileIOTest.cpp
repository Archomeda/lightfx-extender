#include "../../LFXE/Utils/FileIO.h"
#include "CppUnitTest.h"

// Windows includes
#include <ShlObj.h>


using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lightfx::utils;

namespace lightfx_tests {
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

    TEST_METHOD(FileDoesExist) {
        wstring file = L"FileThatDoesExist.txt";
        HANDLE hFile = CreateFileW(file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        CloseHandle(hFile);

        bool exists = FileExists(file);
        DeleteFileW(file.c_str());
        Assert::IsTrue(exists);
    }

    TEST_METHOD(FileDoesNotExist) {
        wstring file = L"FileThatDoesNotExist.txt";
        Assert::IsFalse(FileExists(file));
    }

    TEST_METHOD(CreateDirectoryIfNotExists) {
        wstring dir = L"Some\\Nested\\Dir";
        Assert::IsTrue(CreateDirIfNotExists(dir));
        bool exists = DirExists(dir);
        RemoveDirectoryW(L"Some\\Nested\\Dir");
        RemoveDirectoryW(L"Some\\Nested");
        RemoveDirectoryW(L"Some");
        Assert::IsTrue(exists);
    }

    TEST_METHOD(RenameExistingFile) {
        wstring file = L"SomeRandomFile.txt";
        wstring target = L"SomeRandomFile2.txt";
        HANDLE hFile = CreateFileW(file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        CloseHandle(hFile);

        Assert::IsTrue(FileExists(file));
        bool renamed = RenameFile(file, target);
        bool exists = FileExists(target);
        if (renamed) {
            DeleteFileW(target.c_str());
        } else {
            DeleteFileW(file.c_str());
        }
        Assert::IsTrue(renamed);
        Assert::IsTrue(exists);
    }

    TEST_METHOD(RemoveExistingFile) {
        wstring file = L"SomeRandomFile.txt";
        HANDLE hFile = CreateFileW(file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        CloseHandle(hFile);

        Assert::IsTrue(FileExists(file));
        bool removed = RemoveFile(file);
        bool exists = FileExists(file);
        if (!removed || exists) {
            DeleteFileW(file.c_str());
        }
        Assert::IsTrue(removed);
        Assert::IsFalse(exists);
    }

    TEST_METHOD(StorageDirectoryNotEmpty) {
        Assert::IsFalse(GetDataStorageFolder() == L"");
    }

    TEST_METHOD(ProgramDirectoryNotEmpty) {
        Assert::IsFalse(GetProgramFolder() == L"");
    }

        };
    }
}

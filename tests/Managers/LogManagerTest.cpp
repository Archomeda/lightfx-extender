#include "CppUnitTest.h"
#include "../../src/Managers/LogManager.h"

// Windows includes
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx::managers;

namespace lightfx_tests {
    namespace managers {

        TEST_CLASS(LogManagerTest) {
public:

    TEST_METHOD(GetLogFileNameIsNotEmpty) {
        auto manager = make_shared<LogManager>();
        wstring fileName = manager->GetLogFileName();
        Assert::IsFalse(fileName == L"", L"File name empty");
        Assert::IsTrue(fileName.length() > 0, L"File length is 0");
    }

    TEST_METHOD(SetLogDirectory) {
        auto manager = make_shared<LogManager>();
        wstring directory = L"SomeDirectoryHere";
        manager->SetLogDirectory(directory);
        Assert::IsTrue(directory == manager->GetLogDirectory());
    }

    TEST_METHOD(Log) {
        auto manager = make_shared<LogManager>();
        manager->SetLogDirectory(L"./");
        manager->Log(LogLevel::Info, L"Test log line");

        BOOL exists = PathFileExistsW(manager->GetLogFileName().c_str());
        DeleteFileW(manager->GetLogFileName().c_str());
        Assert::AreEqual(TRUE, exists);
    }


    TEST_METHOD(LogLastWindowsError) {
        auto manager = make_shared<LogManager>();
        manager->SetLogDirectory(L"./");
        manager->LogLastWindowsError();

        BOOL exists = PathFileExistsW(manager->GetLogFileName().c_str());
        DeleteFileW(manager->GetLogFileName().c_str());
        Assert::AreEqual(TRUE, exists);
    }

        };
    }
}

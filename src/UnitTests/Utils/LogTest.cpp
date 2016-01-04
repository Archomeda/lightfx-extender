#include "../../LFXE/Utils/Log.h"
#include "CppUnitTest.h"

// Windows includes
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx::utils;

namespace lightfx_tests {
    namespace utils {

        TEST_CLASS(LogTest) {
public:

    TEST_METHOD(GetLogFileNameIsNotEmpty) {
        wstring fileName = Log::GetLogFileName();
        Assert::IsFalse(fileName == L"", L"File name empty");
        Assert::IsTrue(fileName.length() > 0, L"File length is 0");
    }

    TEST_METHOD(SetLogDirectory) {
        wstring directory = L"SomeDirectoryHere";
        Log::SetLogDirectory(directory);
        Assert::IsTrue(directory == Log::GetLogDirectory());
    }

    TEST_METHOD(SetMinimumLogLevel) {
        LogLevel logLevel = LogLevel::Error;
        Log::SetMinimumLogLevel(logLevel);
        Assert::IsTrue(logLevel == Log::GetMinimumLogLevel());
    }

    TEST_METHOD(Log_) {
        Log::SetLogDirectory(L"./");
        Log::SetMinimumLogLevel(LogLevel::Debug);

        Log::LogLine(LogLevel::Info, __FILE__, __LINE__, __FUNCTION__, L"Test log line");

        BOOL exists = PathFileExistsW(Log::GetLogFileName().c_str());
        DeleteFileW(Log::GetLogFileName().c_str());
        Assert::AreEqual(TRUE, exists);
    }


    TEST_METHOD(LogLastWindowsError) {
        Log::SetLogDirectory(L"./");
        Log::SetMinimumLogLevel(LogLevel::Debug);

        Log::LogLastWindowsError(__FILE__, __LINE__, __FUNCTION__);

        BOOL exists = PathFileExistsW(Log::GetLogFileName().c_str());
        DeleteFileW(Log::GetLogFileName().c_str());
        Assert::AreEqual(TRUE, exists);
    }

        };
    }
}

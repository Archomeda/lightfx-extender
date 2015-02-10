#include "CppUnitTest.h"
#include "../../src/Config/MainConfigFile.h"

// Standard includes
#include <string>
#include <iostream>
#include <fstream>
#include <codecvt>

// Windows includes
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::config;
using namespace lightfx::managers;

namespace config {
    TEST_CLASS(MainConfigFileTest) {
public:

    TEST_METHOD(SetConfigManager) {
        auto configManager = make_shared<ConfigManager>();
        MainConfigFile configFile;
        configFile.SetConfigManager(configManager->shared_from_this());
        Assert::IsTrue(configManager == configFile.GetConfigManager());
    }

    TEST_METHOD(SetConfigDirectory) {
        wstring directoryName = L"SomeNiceDirectoryHere";
        MainConfigFile configFile;
        configFile.SetConfigDirectory(directoryName);
        Assert::AreEqual<wstring>(directoryName, configFile.GetConfigDirectory());
    }


    TEST_METHOD(LoadFileNonExisting) {
        wstring filename = L"NonExistingFile.conf";
        MainConfigFile configFile;
        configFile.SetConfigDirectory(L"./");
        configFile.Load(filename);
        Assert::AreEqual<wstring>(filename, configFile.GetCurrentFileName());
    }

    TEST_METHOD(LoadFileExisting) {
        wstring filename = L"ConfigFile.conf";

        wofstream stream;
        stream.imbue(locale(stream.getloc(), new codecvt_utf8<wchar_t>));
        stream.open(filename, wios::out | wios::app | wios::binary);
        stream << "{}";

        MainConfigFile configFile;
        configFile.SetConfigDirectory(L"./");
        configFile.Load(filename);
        Assert::AreEqual<wstring>(filename, configFile.GetCurrentFileName());
    }

    TEST_METHOD(SaveFileNonExisting) {
        wstring filename = L"NonExistingFile.conf";
        MainConfigFile configFile;
        configFile.SetConfigDirectory(L"./");
        configFile.Load(filename);
        DeleteFileW(filename.c_str());
        configFile.Save();

        BOOL exists = SUCCEEDED(PathFileExistsW(filename.c_str()));
        DeleteFileW(filename.c_str());
        Assert::AreEqual(TRUE, exists);
    }

    TEST_METHOD(SaveFileExisting) {
        wstring filename = L"ConfigFile.conf";

        wofstream stream;
        stream.imbue(locale(stream.getloc(), new codecvt_utf8<wchar_t>));
        stream.open(filename, wios::out | wios::app | wios::binary);
        stream << "{}";
        stream.close();

        MainConfigFile configFile;
        configFile.SetConfigDirectory(L"./");
        configFile.Load(filename);
        DeleteFileW(filename.c_str());
        configFile.Save();

        BOOL exists = PathFileExistsW(filename.c_str());
        DeleteFileW(filename.c_str());
        Assert::AreEqual(TRUE, exists);
    }

    TEST_METHOD(SaveFileToOther) {
        wstring filename = L"ConfigFile.conf";
        wstring filename2 = L"ConfigFile2.conf";

        wofstream stream;
        stream.imbue(locale(stream.getloc(), new codecvt_utf8<wchar_t>));
        stream.open(filename, wios::out | wios::app | wios::binary);
        stream << "{}";
        stream.close();

        MainConfigFile configFile;
        configFile.SetConfigDirectory(L"./");
        configFile.Load(filename);
        DeleteFileW(filename.c_str());
        configFile.Save(filename2);

        BOOL exists = PathFileExistsW(filename2.c_str());
        DeleteFileW(filename2.c_str());
        Assert::AreEqual(TRUE, exists);
    }

    };

}

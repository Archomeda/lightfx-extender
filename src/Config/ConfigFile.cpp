#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "ConfigFile.h"

// Standard includes
#include <iostream>
#include <fstream>
#include <codecvt>
#include <sstream>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#include <ShlObj.h>

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/FileIO.h"
#include "../Utils/Log.h"
#include "../Utils/String.h"


#define LOG(logLevel, message) LOG_(logLevel, wstring(L"Config ") + this->GetCurrentFileName() + L" - " + message)

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::utils;

namespace lightfx {
    namespace config {

        LFXE_API ConfigFile::ConfigFile() {
            this->configDirectory = GetDataStorageFolder();
        }


        LFXE_API void ConfigFile::Load(const wstring& fileName) {
            this->configFileName = fileName;

            // Load defaults first, in case the config file does not exist yet
            this->LoadDefaults();

            // Load the config file
            try {
                wstring filePath = this->GetConfigDirectory();
                if (DirExists(filePath)) {
                    filePath += L"/" + fileName;
                    wifstream configStream;
                    configStream.imbue(locale(configStream.getloc(), new codecvt_utf8<wchar_t>));
                    configStream.open(filePath, wios::in | wios::binary);

                    wstringstream stream;
                    stream << configStream.rdbuf();

                    wstring data = stream.str();
                    configStream.close();

                    // Deserialize this stuff
                    this->Deserialize(data);

                    LOG(LogLevel::Info, L"Loaded configuration");
                } else {
                    LOG(LogLevel::Info, L"No configuration found, using defaults");
                }
            } catch (exception& e) {
                LOG(LogLevel::Warning, L"Exception while loading configuration: " + string_to_wstring(e.what()));
            }
        }

        LFXE_API void ConfigFile::Save() {
            this->Save(this->GetCurrentFileName());
        }

        LFXE_API void ConfigFile::Save(const wstring& fileName) {
            // Serialize config first
            wstring data = this->Serialize();

            try {
                wstring filePath = this->GetConfigDirectory();
                if (!DirExists(filePath)) {
                    if (CreateDirectoryW(filePath.c_str(), NULL) == FALSE) {
                        // TODO: Log directory creation failure
                        return;
                    }
                }
                filePath += L"/" + fileName;

                wofstream configStream;
                configStream.imbue(locale(configStream.getloc(), new codecvt_utf8<wchar_t>));
                configStream.open(filePath, wios::out | wios::binary);
                configStream << data;
                configStream.close();

                LOG(LogLevel::Debug, L"Saved configuration");
            } catch (exception& e) {
                LOG(LogLevel::Warning, L"Exception while saving configuration: " + string_to_wstring(e.what()));
            }
        }


        LFXE_API const std::wstring ConfigFile::GetCurrentFileName() {
            return this->configFileName;
        }

        LFXE_API const std::wstring ConfigFile::GetConfigDirectory() {
            return this->configDirectory;
        }

        LFXE_API void ConfigFile::SetConfigDirectory(const std::wstring& directory) {
            this->configDirectory = directory;
        }

    }
}

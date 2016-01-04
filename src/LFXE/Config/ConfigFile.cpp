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
                wstring dirPath = this->GetConfigDirectory();
                wstring filePath = dirPath + L"\\" + fileName;
                if (DirExists(dirPath) && FileExists(filePath)) {
                    wifstream configStream;
                    configStream.imbue(locale(configStream.getloc(), new codecvt_utf8<wchar_t>));
                    configStream.open(filePath, wios::in | wios::binary);

                    wstringstream stream;
                    stream << configStream.rdbuf();

                    wstring data = stream.str();
                    configStream.close();

                    // Deserialize this stuff
                    this->Deserialize(data);

                    this->configLoaded = true;
                    LOG_INFO(L"Loaded configuration " + fileName);
                } else {
                    this->configLoaded = false;
                    LOG_INFO(L"Configuration " + fileName + L" not found, using defaults");
                }
            } catch (exception& e) {
                LOG_WARNING(L"Exception while loading configuration " + fileName + L", using defaults instead: " + string_to_wstring(e.what()));
                this->configLoaded = false;
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
                        //TODO: Log directory creation failure
                        return;
                    }
                }
                filePath += L"/" + fileName;

                wofstream configStream;
                configStream.imbue(locale(configStream.getloc(), new codecvt_utf8<wchar_t>));
                configStream.open(filePath, wios::out | wios::binary);
                configStream << data;
                configStream.close();

                LOG_DEBUG(L"Saved configuration " + fileName);
            } catch (exception& e) {
                LOG_WARNING(L"Exception while saving configuration " + fileName + L", settings not saved: " + string_to_wstring(e.what()));
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

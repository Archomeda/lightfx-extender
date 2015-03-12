#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LogManager.h"

// Standard includes
#include <iostream>
#include <fstream>
#include <codecvt>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#include <time.h>

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/ConfigManager.h"
#include "../Config/MainConfigFile.h"
#include "../Utils/FileIO.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API LogManager::LogManager() {
            this->logDirectory = GetDataStorageFolder();
        }


        LFXE_API void LogManager::Log(const LogLevel logLevel, const std::wstring& line) {
            // Check if the log level is equal or higher than the minimum log level that's currently set
            if (this->minimumLogLevel > logLevel) {
                return;
            }

            // Get a nice date/time prefix first
            wchar_t buff[20];
            time_t t = time(0);
            tm lt;
            localtime_s(&lt, &t);
            wcsftime(buff, 20, L"%Y-%m-%d %H:%M:%S", &lt);
            wstring timePrefix(buff);

            // Determine the log level prefix
            wstring logLevelPrefix;
            switch (logLevel) {
            case LogLevel::Debug:
                logLevelPrefix = L"[DEBUG]";
                break;
            case LogLevel::Info:
                logLevelPrefix = L"[INFO]";
                break;
            case LogLevel::Warning:
                logLevelPrefix = L"[WARNING]";
                break;
            case LogLevel::Error:
                logLevelPrefix = L"[ERROR]";
                break;
            }

            // Log
            try {
                wstring filePath = this->GetLogDirectory();
                if (!DirExists(filePath)) {
                    if (CreateDirectoryW(filePath.c_str(), NULL) == FALSE) {
                        // Can't log the failure, since we just failed to create the directory where the log file should be...
                        return;
                    }
                }
                filePath += L"/" + this->logFileName;
                wofstream logStream;
                logStream.imbue(locale(logStream.getloc(), new codecvt_utf8<wchar_t>));
                logStream.open(filePath, wios::out | wios::binary | wios::app);
                logStream << timePrefix << L" - " << logLevelPrefix << L" " << line << std::endl;
                logStream.close();
            } catch (...) {
                // Can't log the exception, since we just failed to log something else...
            }
        }

        LFXE_API void LogManager::LogLastWindowsError() {
            wstring message;

            //Get the error message, if any.
            DWORD errorMessageID = GetLastError();
            if (errorMessageID == 0) {
                return this->Log(LogLevel::Debug, L"No Windows error found");
            }

            LPWSTR messageBuffer = nullptr;
            size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&messageBuffer, 0, NULL);

            message = wstring(messageBuffer, size);

            //Free the buffer.
            LocalFree(messageBuffer);

            return Log(LogLevel::Error, L"Windows error: " + message);
        }


        LFXE_API void LogManager::RotateLog() {
            wstring filePath = this->GetLogDirectory();
            if (DirExists(filePath)) {
                filePath += L"/" + this->logFileName;
                if (FileExists(filePath)) {
                    for (int i = 3; i >= 0; --i) {
                        wstring filePathOld = filePath + L"." + to_wstring(i);
                        wstring filePathNew = filePath + L"." + to_wstring(i + 1);
                        if (FileExists(filePathOld)) {
                            if (i == 3) {
                                DeleteFileW(filePathOld.c_str());
                            } else {
                                MoveFileW(filePathOld.c_str(), filePathNew.c_str());
                            }
                        }
                    }
                    MoveFileW(filePath.c_str(), (filePath + L".0").c_str());
                }
            }
        }


        LFXE_API const wstring LogManager::GetLogFileName() {
            return this->logFileName;
        }

        LFXE_API const wstring LogManager::GetLogDirectory() {
            return this->logDirectory;
        }

        LFXE_API void LogManager::SetLogDirectory(const wstring& directory) {
            this->logDirectory = directory;
        }


        LFXE_API LogLevel LogManager::GetMinimumLogLevel() {
            return this->minimumLogLevel;
        }

        LFXE_API void LogManager::SetMinimumLogLevel(const LogLevel logLevel) {
            this->minimumLogLevel = logLevel;
        }
    }
}

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
#include "../Utils/FileIO.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API LogManager::LogManager() {
            this->logDirectory = GetDataStorageFolder();
        }


        LFXE_API void LogManager::Log(const LogLevel logLevel, const std::wstring& line) {
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
                        // TODO: Log directory creation failure (tbh a bit useless since we just failed to create the directory where the log file should be...)
                        return;
                    }
                }
                filePath += this->logFileName;
                wofstream logStream;
                logStream.imbue(locale(logStream.getloc(), new codecvt_utf8<wchar_t>));
                logStream.open(filePath, wios::out | wios::binary);
                logStream << timePrefix << L" - " << logLevelPrefix << L" " << line << std::endl;
                logStream.close();
            } catch (...) {
                // TODO: Log error
            }
        }

        LFXE_API void LogManager::LogLastWindowsError() {
            wstring message;

            //Get the error message, if any.
            DWORD errorMessageID = GetLastError();
            if (errorMessageID == 0) {
                return this->Log(LogLevel::Info, L"No Windows error found");
            }

            LPWSTR messageBuffer = nullptr;
            size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&messageBuffer, 0, NULL);

            message = wstring(messageBuffer, size);

            //Free the buffer.
            LocalFree(messageBuffer);

            return Log(LogLevel::Error, L"Windows error: " + message);
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

    }
}
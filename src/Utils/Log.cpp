#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "Log.h"

// Standard includes
#include <codecvt>
#include <fstream>
#include <iostream>
#include <mutex>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#include <time.h>

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/FileIO.h"


using namespace std;

namespace lightfx {
    namespace utils {

        wstring logFileName = L"LightFXExtender.log";
        wstring logDirectory = GetDataStorageFolder();
#ifdef LFXE_TESTING
        LogLevel minimumLogLevel = LogLevel::Off;
#else
        LogLevel minimumLogLevel = LogLevel::Info;
#endif
        mutex logMutex;


        LFXE_API void Log::LogLine(const LogLevel logLevel, const std::wstring& line) {
            // Check if the log level is equal or higher than the minimum log level that's currently set
            if (minimumLogLevel > logLevel) {
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
                wstring filePath = logDirectory;
                if (!DirExists(filePath)) {
                    if (CreateDirectoryW(filePath.c_str(), NULL) == FALSE) {
                        // Can't log the failure, since we just failed to create the directory where the log file should be...
                        return;
                    }
                }
                filePath += L"/" + logFileName;
                wofstream logStream;
                logStream.imbue(locale(logStream.getloc(), new codecvt_utf8<wchar_t>));
                logMutex.lock(); // Exclusive lock on log file
                logStream.open(filePath, wios::out | wios::binary | wios::app);
                logStream << timePrefix << L" - " << logLevelPrefix << L" " << line << std::endl;
                logStream.close();
                logMutex.unlock(); // Release lock
            } catch (...) {
                // Can't log the exception, since we just failed to log something else...
            }
        }

        LFXE_API void Log::LogLastWindowsError() {
            wstring message;

            //Get the error message, if any.
            DWORD errorMessageID = GetLastError();
            if (errorMessageID == 0) {
                LOG_(LogLevel::Debug, L"No Windows error found");
            }

            LPWSTR messageBuffer = nullptr;
            size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&messageBuffer, 0, NULL);

            message = wstring(messageBuffer, size);

            //Free the buffer.
            LocalFree(messageBuffer);

            LOG_(LogLevel::Error, L"Windows error: " + message);
        }


        LFXE_API void Log::RotateLog() {
            wstring filePath = logDirectory;
            if (DirExists(filePath)) {
                filePath += L"/" + logFileName;
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


        LFXE_API const wstring Log::GetLogFileName() {
            return logFileName;
        }

        LFXE_API const wstring Log::GetLogDirectory() {
            return logDirectory;
        }

        LFXE_API void Log::SetLogDirectory(const wstring& directory) {
            logDirectory = directory;
        }


        LFXE_API LogLevel Log::GetMinimumLogLevel() {
            return minimumLogLevel;
        }

        LFXE_API void Log::SetMinimumLogLevel(const LogLevel logLevel) {
            minimumLogLevel = logLevel;
        }
    }
}

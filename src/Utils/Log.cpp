#include "Log.h"

// Standard includes
#include <iostream>
#include <fstream>
#include <codecvt>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#include <time.h>

// Project includes
#include "FileIO.h"


#define APPDATA_LOGFILE L"LightFX.log"

using namespace std;
using namespace lightfx;
using namespace lightfx::utils;


#define DTTMFMT L"%Y-%m-%d %H:%M:%S"
#define DTTMSZ 21
static wstring getDtTm() {
    wchar_t buff[DTTMSZ];
    time_t t = time(0);
    tm lt;
    localtime_s(&lt, &t);
    wcsftime(buff, DTTMSZ, DTTMFMT, &lt);
    return wstring(buff);
}


namespace lightfx {
    namespace utils {

        void Log(wstring line) {
            wstring configPath;
            if (GetRoamingAppDataFolder(configPath)) {
                configPath = configPath + L"/" + APPDATA_FOLDER;
                if (!DirExists(configPath)) {
                    if (CreateDirectoryW(configPath.c_str(), NULL) == FALSE) {
                        return;
                    }
                }
                configPath = configPath + L"/" + APPDATA_LOGFILE;

                wofstream logstream;
                logstream.imbue(locale(logstream.getloc(), new codecvt_utf8<wchar_t>));
                logstream.open(configPath, wios::out | wios::app | wios::binary);
                logstream << getDtTm() << L" - " << line << std::endl;
            }
        }

        void LogLastError() {
            wstring message;

            //Get the error message, if any.
            DWORD errorMessageID = GetLastError();
            if (errorMessageID == 0)
                message = L"No error found";

            LPWSTR messageBuffer = nullptr;
            size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

            message = wstring(messageBuffer, size);

            //Free the buffer.
            LocalFree(messageBuffer);

            return Log(message);
        }

    }
}

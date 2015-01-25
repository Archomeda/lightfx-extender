#include "Log.h"

// Standard includes
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

// Windows includes
#include <Windows.h>
#include <time.h>

// Project includes
#include "FileIO.h"


#define wstring_to_string(str) wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(str)
#define string_to_wstring(str) wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(str)

#define APPDATA_LOGFILE L"LightFX.log"

using namespace std;
using namespace lightfx;
using namespace lightfx::utils;


#define DTTMFMT "%Y-%m-%d %H:%M:%S "
#define DTTMSZ 21
static string getDtTm() {
    char buff[DTTMSZ];
    time_t t = time(0);
    tm lt;
    localtime_s(&lt, &t);
    strftime(buff, DTTMSZ, DTTMFMT, &lt);
    return string(buff);
}


namespace lightfx {
    namespace utils {

        void Log(string line) {
            wstring configPath;
            if (GetRoamingAppDataFolder(configPath)) {
                configPath = configPath + L"/" + APPDATA_FOLDER;
                if (!DirExists(configPath)) {
                    if (CreateDirectoryW(configPath.c_str(), NULL) == FALSE) {
                        return;
                    }
                }
                configPath = configPath + L"/" + APPDATA_LOGFILE;

                ofstream logstream;
                logstream.open(configPath, ios::out | ios::app | ios::binary);
                logstream << getDtTm() << line << "\n";
            }
        }

        void Log(wstring line) {
            Log(wstring_to_string(line));
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

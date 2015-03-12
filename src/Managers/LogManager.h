#pragma once

// Standard includes
#include <string>

// Project includes
#include "Manager.h"

// API exports
#include "../Common/ApiExports.h"


#define LOG_(obj, logLevel, message) {              \
    if (obj->GetMinimumLogLevel() <= logLevel) {    \
        obj->Log(logLevel, message); } }

#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    class LightFXExtender;

    namespace managers {

        enum LogLevel {
            Debug,
            Info,
            Warning,
            Error
        };

        class LFXE_API LogManager : public Manager < LogManager > {

        public:
            LogManager();

            void Log(const LogLevel logLevel, const std::wstring& line);
            void LogLastWindowsError();

            void RotateLog();

            virtual const std::wstring GetLogFileName();
            virtual const std::wstring GetLogDirectory();
            virtual void SetLogDirectory(const std::wstring& directory);

            virtual LogLevel GetMinimumLogLevel();
            virtual void SetMinimumLogLevel(const LogLevel logLevel);

        private:
            std::wstring logFileName = L"LightFXExtender.log";
            std::wstring logDirectory;
            LogLevel minimumLogLevel = LogLevel::Info;

        };

    }
}

#pragma warning(pop)

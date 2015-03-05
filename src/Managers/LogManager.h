#pragma once

// Standard includes
#include <string>

// Project includes
#include "Manager.h"

// API exports
#include "../Common/ApiExports.h"


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

            virtual const std::wstring GetLogFileName();
            virtual const std::wstring GetLogDirectory();
            virtual void SetLogDirectory(const std::wstring& directory);

        private:
            std::wstring logFileName = L"LightFXExtender.log";
            std::wstring logDirectory;

        };

    }
}

#pragma warning(pop)

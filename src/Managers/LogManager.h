#pragma once

// Standard includes
#include <string>

// Project includes
#include "../LightFXExtender.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    class LightFXExtender;

    namespace managers {

        enum LogLevel {
            Info,
            Warning,
            Error
        };

        class LFXE_API LogManager {

        public:
            LogManager(const LightFXExtender* lightFXExtender);
            ~LogManager();

            void Log(const LogLevel logLevel, const std::wstring& line);
            void LogLastWindowsError();

        };

    }
}

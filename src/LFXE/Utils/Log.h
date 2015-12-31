#pragma once

// Standard includes
#include <chrono>
#include <string>

// API exports
#include "../Common/ApiExports.h"


#define LOG_(logLevel, message) {                                       \
    if (::lightfx::utils::Log::GetMinimumLogLevel() <= logLevel) {      \
        ::lightfx::utils::Log::LogLineAsync(logLevel, message); } }

#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace utils {

        enum LogLevel {
            Debug,
            Info,
            Warning,
            Error,
            Off = 99
        };

        class LFXE_API Log {

        public:
            static void StartLoggerWorker();
            static void StopLoggerWorker();

            static void LogLine(const LogLevel logLevel, const std::wstring& line);
            static void LogLastWindowsError();
            static void LogLineAsync(const LogLevel logLevel, const std::wstring& line);
            static void LogLastWindowsErrorAsync();

            static void RotateLog();

            static const std::wstring GetLogFileName();
            static const std::wstring GetLogDirectory();
            static void SetLogDirectory(const std::wstring& directory);

            static LogLevel GetMinimumLogLevel();
            static void SetMinimumLogLevel(const LogLevel logLevel);

        private:
            static void LoggerWorker();

            static std::wofstream OpenStream();
            static std::wstring GetLine(const LogLevel logLevel, const std::wstring& line, std::chrono::milliseconds logTime);
            static void WriteBacklog();

        };

    }
}

#pragma warning(pop)

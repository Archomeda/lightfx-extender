#pragma once

// Standard includes
#include <chrono>
#include <string>

// API exports
#include "../Common/ApiExports.h"


#define LOG(logLevel, message) {                                        \
    if (::lightfx::utils::Log::GetMinimumLogLevel() <= logLevel) {      \
        ::lightfx::utils::Log::LogLineAsync(logLevel, __FILE__, __LINE__, __FUNCTION__, message); } }

#define LOG_DEBUG(message) LOG(::lightfx::utils::LogLevel::Debug, message)
#define LOG_INFO(message) LOG(::lightfx::utils::LogLevel::Info, message)
#define LOG_WARNING(message) LOG(::lightfx::utils::LogLevel::Warning, message)
#define LOG_ERROR(message) LOG(::lightfx::utils::LogLevel::Error, message)
#define LOG_WINERROR() ::lightfx::utils::Log::LogLastWindowsErrorAsync(__FILE__, __LINE__, __FUNCTION__)

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

        struct LFXE_API LogMessage {
            LogLevel level;
            std::wstring file;
            int line;
            std::wstring function;
            std::wstring message;
            std::chrono::milliseconds time;
        };

        class LFXE_API Log {

        public:
            static void StartLoggerWorker();
            static void StopLoggerWorker();

            static void LogLine(const LogLevel logLevel, const std::string& file, const int line, const std::string& function, const std::wstring& message);
            static void LogLastWindowsError(const std::string& file, const int line, const std::string& function);
            static void LogLineAsync(const LogLevel logLevel, const std::string& file, const int line, const std::string& function, const std::wstring& message);
            static void LogLastWindowsErrorAsync(const std::string& file, const int line, const std::string& function);

            static void RotateLog();

            static const std::wstring GetLogFileName();
            static const std::wstring GetLogDirectory();
            static void SetLogDirectory(const std::wstring& directory);

            static LogLevel GetMinimumLogLevel();
            static void SetMinimumLogLevel(const LogLevel logLevel);

        private:
            static void LoggerWorker();

            static std::wofstream OpenStream();
            static std::wstring GetLine(const LogMessage& message);
            static void WriteBacklog();

        };

    }
}

#pragma warning(pop)

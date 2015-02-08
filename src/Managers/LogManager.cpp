#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LogManager.h"


using namespace std;

namespace lightfx {
    namespace managers {

        LFXE_API LogManager::LogManager(const LightFXExtender* lightFXExtender) {

        }

        LFXE_API LogManager::~LogManager() {

        }


        LFXE_API void LogManager::Log(const LogLevel logLevel, const std::wstring& line) {

        }

        LFXE_API void LogManager::LogLastWindowsError() {

        }

    }
}
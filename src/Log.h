#pragma once
#include <string>

namespace lightfx {
    namespace log {
        
        void Log(std::string line);
        void Log(std::wstring line);
        void LogLastError();

    }
}

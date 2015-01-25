#pragma once

// Standard includes
#include <string>


namespace lightfx {
    namespace utils {
        
        void Log(std::string line);
        void Log(std::wstring line);
        void LogLastError();

    }
}

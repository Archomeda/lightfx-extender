#pragma once

// Standard includes
#include <string>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {
    namespace config {

        class LFXE_API ConfigFile {

        public:
            ConfigFile();

            bool LoadDefaults();
            bool LoadFile(const std::wstring& fileName);
            bool SaveFile(const std::wstring& fileName);

            const std::wstring GetCurrentFileName();

        private:
            std::wstring configFilename;

        };

    }
}

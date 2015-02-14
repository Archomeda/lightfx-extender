#pragma once

// Standard includes
#include <string>

// Project includes
#include "../../src/Config/ConfigFile.h"


namespace lightfx_tests {
    namespace config {

        class ConfigFileMock : public lightfx::config::ConfigFile {

        protected:
            virtual void LoadDefaults() override {}
            virtual std::wstring Serialize() override { return L""; }
            virtual void Deserialize(const std::wstring& data) override {}

        };

    }
}

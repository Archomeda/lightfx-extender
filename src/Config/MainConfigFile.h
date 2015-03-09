#pragma once

// Standard includes
#include <string>

// 3rd party includes
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

// Project includes
#include "ConfigFile.h"
#include "../Managers/LogManager.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace config {

        class LFXE_API MainConfigFile : public ConfigFile {

        public:
            bool AutoUpdatesEnabled = true;
            managers::LogLevel MinimumLogLevel = managers::LogLevel::Info;

            std::map<std::wstring, bool> EnabledDevices = {};

            bool TrayIconEnabled = true;
            bool TrayIconUseGameIcon = false;

            int LogitechColorRangeOutMin = 0;
            int LogitechColorRangeOutMax = 255;
            int LogitechColorRangeInMin = 0;
            int LogitechColorRangeInMax = 100;

            std::wstring LightpackHost = L"127.0.0.1";
            std::wstring LightpackPort = L"3636";
            std::wstring LightpackKey = L"";

            bool GuildWars2TeamColorEnabled = true;
            std::wstring GuildWars2TeamColorAnimation = L"Pulse";

            void Load();
            using ConfigFile::Load;

        protected:
            virtual void LoadDefaults() override;
            virtual std::wstring Serialize() override;
            virtual void Deserialize(const std::wstring& data) override;

        private:
            typedef rapidjson::GenericDocument<rapidjson::UTF16<>> WDocument;
            typedef rapidjson::GenericValue<rapidjson::UTF16<>> WValue;
            typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<>> WStringBuffer;
            typedef rapidjson::PrettyWriter<WStringBuffer, rapidjson::UTF16<>, rapidjson::UTF16<>> WPrettyWriter;
            WDocument doc;

            WValue MakeJsonWString(const std::wstring& str, WDocument::AllocatorType& allocator);

        };

    }
}

#pragma warning(pop)

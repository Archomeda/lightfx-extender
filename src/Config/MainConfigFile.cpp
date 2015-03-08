#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "MainConfigFile.h"

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/DeviceManager.h"


#pragma region Configuration keys
#define CONF_AUTOUPDATESENABLED L"AutoUpdatesEnabled"
#define CONF_MINIMUMLOGLEVEL L"MinimumLogLevel"
#define CONF_ENABLEDDEVICES L"EnabledDevices"

#define CONF_LOGITECHCOLORRANGE L"LogitechColorRange"
#define CONF_LOGITECHCOLORRANGE_OUTMIN L"AlienFXMin"
#define CONF_LOGITECHCOLORRANGE_OUTMAX L"AlienFXMax"
#define CONF_LOGITECHCOLORRANGE_INMIN L"LogitechMin"
#define CONF_LOGITECHCOLORRANGE_INMAX L"LogitechMax"

#define CONF_LIGHTPACKAPI L"LightpackAPI"
#define CONF_LIGHTPACKHOST L"Hostname"
#define CONF_LIGHTPACKPORT L"PortNumber"
#define CONF_LIGHTPACKKEY L"Key"

#define CONF_GUILDWARS2 L"GuildWars2"
#define CONF_GUILDWARS2_TEAMCOLORENABLED L"TeamColorEnabled"
#define CONF_GUILDWARS2_TEAMCOLORANIMATION L"TeamColorAnimation"
#pragma endregion

using namespace std;
using namespace rapidjson;
using namespace lightfx::managers;

namespace lightfx {
    namespace config {

        LFXE_API void MainConfigFile::Load() {
            ConfigFile::Load(L"settings.json");
        }

        LFXE_API void MainConfigFile::LoadDefaults() {
            this->MinimumLogLevel = LogLevel::Info;

            this->LogitechColorRangeOutMin = 0;
            this->LogitechColorRangeOutMax = 255;
            this->LogitechColorRangeInMin = 0;
            this->LogitechColorRangeInMax = 100;

            this->LightpackHost = L"127.0.0.1";
            this->LightpackPort = L"3636";
            this->LightpackKey = L"";

            this->GuildWars2TeamColorEnabled = true;
            this->GuildWars2TeamColorAnimation = L"Pulse";
        }

        LFXE_API wstring MainConfigFile::Serialize() {
            WDocument::AllocatorType& allocator = this->doc.GetAllocator();
            this->doc.SetObject();

            // Auto updates enabled
            this->doc.AddMember(CONF_AUTOUPDATESENABLED, this->AutoUpdatesEnabled, allocator);

            // Minimum log level
            WValue minimumLogLevel;
            switch (this->MinimumLogLevel) {
            case LogLevel::Debug:
                minimumLogLevel = this->MakeJsonWString(L"Debug", allocator);
                break;
            case LogLevel::Warning:
                minimumLogLevel = this->MakeJsonWString(L"Warning", allocator);
                break;
            case LogLevel::Error:
                minimumLogLevel = this->MakeJsonWString(L"Error", allocator);
                break;
            case LogLevel::Info:
            default:
                minimumLogLevel = this->MakeJsonWString(L"Info", allocator);
                break;
            }
            this->doc.AddMember(CONF_MINIMUMLOGLEVEL, minimumLogLevel, allocator);

            // Enabled devices
            WValue enabledDevices(kObjectType);
            auto deviceManager = this->GetManager()->GetLightFXExtender()->GetDeviceManager();
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                enabledDevices.AddMember(WValue(device->GetDeviceName().c_str(), allocator).Move(), WValue(device->IsEnabled()), allocator);
            }
            this->doc.AddMember(CONF_ENABLEDDEVICES, enabledDevices, allocator);

            // Logitech color range
            WValue logitechColorRange(kObjectType);
            logitechColorRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMIN, this->LogitechColorRangeOutMin, allocator);
            logitechColorRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMAX, this->LogitechColorRangeOutMax, allocator);
            logitechColorRange.AddMember(CONF_LOGITECHCOLORRANGE_INMIN, this->LogitechColorRangeInMin, allocator);
            logitechColorRange.AddMember(CONF_LOGITECHCOLORRANGE_INMAX, this->LogitechColorRangeInMax, allocator);
            this->doc.AddMember(CONF_LOGITECHCOLORRANGE, logitechColorRange, allocator);

            // Lightpack API
            WValue lightpackApi(kObjectType);
            lightpackApi.AddMember(CONF_LIGHTPACKHOST, this->MakeJsonWString(this->LightpackHost, allocator), allocator);
            lightpackApi.AddMember(CONF_LIGHTPACKPORT, this->MakeJsonWString(this->LightpackPort, allocator), allocator);
            lightpackApi.AddMember(CONF_LIGHTPACKKEY, this->MakeJsonWString(this->LightpackKey, allocator), allocator);
            this->doc.AddMember(CONF_LIGHTPACKAPI, lightpackApi, allocator);

            // Guild Wars 2
            WValue gw2(kObjectType);
            gw2.AddMember(CONF_GUILDWARS2_TEAMCOLORENABLED, this->GuildWars2TeamColorEnabled, allocator);
            gw2.AddMember(CONF_GUILDWARS2_TEAMCOLORANIMATION, this->MakeJsonWString(this->GuildWars2TeamColorAnimation, allocator), allocator);
            this->doc.AddMember(CONF_GUILDWARS2, gw2, allocator);

            WStringBuffer buffer;
            WPrettyWriter writer(buffer);
            this->doc.Accept(writer);
            return buffer.GetString();
        }

        LFXE_API void MainConfigFile::Deserialize(const wstring& data) {
            this->doc.Parse<0>(data.c_str());

            if (!this->doc.IsObject()) {
                return;
            }

            // Auto updates enabled
            if (this->doc.HasMember(CONF_AUTOUPDATESENABLED) && this->doc[CONF_AUTOUPDATESENABLED].IsBool()) {
                this->AutoUpdatesEnabled = this->doc[CONF_AUTOUPDATESENABLED].GetBool();
            }

            // Minimum log level
            if (this->doc.HasMember(CONF_MINIMUMLOGLEVEL) && this->doc[CONF_MINIMUMLOGLEVEL].IsString()) {
                wstring minimumLogLevel = this->doc[CONF_MINIMUMLOGLEVEL].GetString();
                if (minimumLogLevel == L"Debug") {
                    this->MinimumLogLevel = LogLevel::Debug;
                } else if (minimumLogLevel == L"Info") {
                    this->MinimumLogLevel = LogLevel::Info;
                } else if (minimumLogLevel == L"Warning") {
                    this->MinimumLogLevel = LogLevel::Warning;
                } else if (minimumLogLevel == L"Error") {
                    this->MinimumLogLevel = LogLevel::Error;
                }
            }

            // Enabled devices
            if (this->doc.HasMember(CONF_ENABLEDDEVICES) && this->doc[CONF_ENABLEDDEVICES].IsObject()) {
                const WValue& enabledDevices = this->doc[CONF_ENABLEDDEVICES];
                for (auto itr = enabledDevices.MemberBegin(); itr != enabledDevices.MemberEnd(); ++itr) {
                    if (itr->value.IsBool()) {
                        wstring deviceName = itr->name.GetString();
                        bool deviceEnabled = itr->value.GetBool();
                        this->EnabledDevices[deviceName] = deviceEnabled;
                    }
                }
            }

            // Logitech color range
            if (this->doc.HasMember(CONF_LOGITECHCOLORRANGE) && this->doc[CONF_LOGITECHCOLORRANGE].IsObject()) {
                const WValue& colorRange = this->doc[CONF_LOGITECHCOLORRANGE];
                if (colorRange.HasMember(CONF_LOGITECHCOLORRANGE_OUTMIN) && colorRange[CONF_LOGITECHCOLORRANGE_OUTMIN].IsInt()) {
                    this->LogitechColorRangeOutMin = colorRange[CONF_LOGITECHCOLORRANGE_OUTMIN].GetInt();
                }
                if (colorRange.HasMember(CONF_LOGITECHCOLORRANGE_OUTMAX) && colorRange[CONF_LOGITECHCOLORRANGE_OUTMAX].IsInt()) {
                    this->LogitechColorRangeOutMax = colorRange[CONF_LOGITECHCOLORRANGE_OUTMAX].GetInt();
                }
                if (colorRange.HasMember(CONF_LOGITECHCOLORRANGE_INMIN) && colorRange[CONF_LOGITECHCOLORRANGE_INMIN].IsInt()) {
                    this->LogitechColorRangeInMin = colorRange[CONF_LOGITECHCOLORRANGE_INMIN].GetInt();
                }
                if (colorRange.HasMember(CONF_LOGITECHCOLORRANGE_INMAX) && colorRange[CONF_LOGITECHCOLORRANGE_INMAX].IsInt()) {
                    this->LogitechColorRangeInMax = colorRange[CONF_LOGITECHCOLORRANGE_INMAX].GetInt();
                }
            }

            // Lightpack API
            if (this->doc.HasMember(CONF_LIGHTPACKAPI) && this->doc[CONF_LIGHTPACKAPI].IsObject()) {
                const WValue& api = this->doc[CONF_LIGHTPACKAPI];
                if (api.HasMember(CONF_LIGHTPACKHOST) && api[CONF_LIGHTPACKHOST].IsString()) {
                    this->LightpackHost = api[CONF_LIGHTPACKHOST].GetString();
                }
                if (api.HasMember(CONF_LIGHTPACKPORT) && api[CONF_LIGHTPACKPORT].IsString()) {
                    this->LightpackPort = api[CONF_LIGHTPACKPORT].GetString();
                }
                if (api.HasMember(CONF_LIGHTPACKKEY) && api[CONF_LIGHTPACKKEY].IsString()) {
                    this->LightpackKey = api[CONF_LIGHTPACKKEY].GetString();
                }
            }

            // Guild Wars 2
            if (this->doc.HasMember(CONF_GUILDWARS2) && this->doc[CONF_GUILDWARS2].IsObject()) {
                const WValue& gw2 = this->doc[CONF_GUILDWARS2];
                if (gw2.HasMember(CONF_GUILDWARS2_TEAMCOLORENABLED) && gw2[CONF_GUILDWARS2_TEAMCOLORENABLED].IsBool()) {
                    this->GuildWars2TeamColorEnabled = gw2[CONF_GUILDWARS2_TEAMCOLORENABLED].GetBool();
                }
                if (gw2.HasMember(CONF_GUILDWARS2_TEAMCOLORANIMATION) && gw2[CONF_GUILDWARS2_TEAMCOLORANIMATION].IsString()) {
                    this->GuildWars2TeamColorAnimation = gw2[CONF_GUILDWARS2_TEAMCOLORANIMATION].GetString();
                }
            }
        }

        LFXE_API MainConfigFile::WValue MainConfigFile::MakeJsonWString(const wstring& str, WDocument::AllocatorType& allocator) {
            WValue value(kStringType);
            value.SetString(str.c_str(), allocator);
            return value;
        }

    }
}

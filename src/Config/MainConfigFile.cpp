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
#define CONF_TIMELINE_UPDATE_INTERVAL L"TimelineUpdateInterval"

#define CONF_TRAYICON L"TrayIcon"
#define CONF_TRAYICON_ENABLED L"Enabled"
#define CONF_TRAYICON_USEGAMEICON L"UseGameIcon"

#define CONF_DEVICES L"Devices"
#define CONF_DEVICES_ENABLED L"EnabledDevices"

#define CONF_DEVICES_LIGHTFX L"LightFX"
#define CONF_DEVICES_LIGHTFX_ALIENWAREDLL L"AlienwareDllName"
#define CONF_DEVICES_LIGHTFX_ALIENWAREBACKUPDLL L"AlienwareBackupDllName"

#define CONF_DEVICES_LOGITECH L"Logitech"
#define CONF_DEVICES_LOGITECH_COLORRANGE L"ColorRange"
#define CONF_DEVICES_LOGITECH_COLORRANGE_OUTMIN L"LightFXMin"
#define CONF_DEVICES_LOGITECH_COLORRANGE_OUTMAX L"LightFXMax"
#define CONF_DEVICES_LOGITECH_COLORRANGE_INMIN L"LogitechMin"
#define CONF_DEVICES_LOGITECH_COLORRANGE_INMAX L"LogitechMax"
#define CONF_DEVICES_LOGITECH_RESTORELIGHTSONNULL L"RestoreLightsOnNullEnabled"
#define CONF_DEVICES_LOGITECH_G110WORKAROUND L"G110WorkaroundEnabled"

#define CONF_DEVICES_CORSAIR L"Corsair"
#define CONF_DEVICES_CORSAIR_COLORRANGE L"ColorRange"
#define CONF_DEVICES_CORSAIR_COLORRANGE_OUTMIN L"LightFXMin"
#define CONF_DEVICES_CORSAIR_COLORRANGE_OUTMAX L"LightFXMax"
#define CONF_DEVICES_CORSAIR_COLORRANGE_INMIN L"CorsairMin"
#define CONF_DEVICES_CORSAIR_COLORRANGE_INMAX L"CorsairMax"

#define CONF_DEVICES_LIGHTPACK L"Lightpack"
#define CONF_DEVICES_LIGHTPACK_API L"SocketApi"
#define CONF_DEVICES_LIGHTPACK_API_HOST L"Hostname"
#define CONF_DEVICES_LIGHTPACK_API_PORT L"PortNumber"
#define CONF_DEVICES_LIGHTPACK_API_KEY L"Key"

#define CONF_GAMES L"Games"
#define CONF_GAMES_GUILDWARS2 L"GuildWars2"
#define CONF_GAMES_GUILDWARS2_TEAMCOLORENABLED L"TeamColorEnabled"
#define CONF_GAMES_GUILDWARS2_TEAMCOLORANIMATION L"TeamColorAnimation"
#pragma endregion

using namespace std;
using namespace rapidjson;
using namespace lightfx::managers;
using namespace lightfx::utils;

namespace lightfx {
    namespace config {

        LFXE_API void MainConfigFile::Load() {
            ConfigFile::Load(L"settings.json");
        }

        LFXE_API void MainConfigFile::LoadDefaults() {
            this->AutoUpdatesEnabled = true;
            this->MinimumLogLevel = LogLevel::Info;
            this->TimelineUpdateInterval = 20;

            this->TrayIconEnabled = true;
            this->TrayIconUseGameIcon = false;

            this->AlienwareDllName = L"LightFX.dll";
            this->AlienwareBackupDllName = L"LightFX_.dll";

            this->LightpackHost = L"127.0.0.1";
            this->LightpackPort = L"3636";
            this->LightpackKey = L"";

            this->LogitechColorRangeOutMin = 0;
            this->LogitechColorRangeOutMax = 255;
            this->LogitechColorRangeInMin = 0;
            this->LogitechColorRangeInMax = 100;
			this->LogitechRestoreLightsOnNullEnabled = false;
            this->LogitechG110WorkaroundEnabled = false;

			this->CorsairColorRangeOutMin = 0;
			this->CorsairColorRangeOutMax = 255;
			this->CorsairColorRangeInMin = 0;
			this->CorsairColorRangeInMax = 255;

            this->GuildWars2TeamColorEnabled = true;
            this->GuildWars2TeamColorAnimation = L"Pulse";
        }

        LFXE_API wstring MainConfigFile::Serialize() {
            WDocument::AllocatorType& allocator = this->doc.GetAllocator();
            this->doc.SetObject();

            // Auto updates enabled
            this->doc.AddMember(CONF_AUTOUPDATESENABLED, this->AutoUpdatesEnabled, allocator);

            // Minimum log level
            WValue objMinimumLogLevel;
            switch (this->MinimumLogLevel) {
            case LogLevel::Debug:
                objMinimumLogLevel = this->MakeJsonWString(L"Debug", allocator);
                break;
            case LogLevel::Warning:
                objMinimumLogLevel = this->MakeJsonWString(L"Warning", allocator);
                break;
            case LogLevel::Error:
                objMinimumLogLevel = this->MakeJsonWString(L"Error", allocator);
                break;
            case LogLevel::Info:
            default:
                objMinimumLogLevel = this->MakeJsonWString(L"Info", allocator);
                break;
            }
            this->doc.AddMember(CONF_MINIMUMLOGLEVEL, objMinimumLogLevel, allocator);

            // Timeline update interval
            this->doc.AddMember(CONF_TIMELINE_UPDATE_INTERVAL, this->TimelineUpdateInterval, allocator);

            this->doc.AddMember(CONF_TRAYICON, this->SerializeTrayIcon(allocator), allocator);
            this->doc.AddMember(CONF_DEVICES, this->SerializeDevices(allocator), allocator);
            this->doc.AddMember(CONF_GAMES, this->SerializeGames(allocator), allocator);

            WStringBuffer buffer;
            WPrettyWriter writer(buffer);
            this->doc.Accept(writer);
            return buffer.GetString();
        }

        LFXE_API MainConfigFile::WValue MainConfigFile::SerializeTrayIcon(WDocument::AllocatorType& allocator) {
            WValue obj(kObjectType);
            obj.AddMember(CONF_TRAYICON_ENABLED, this->TrayIconEnabled, allocator);
            obj.AddMember(CONF_TRAYICON_USEGAMEICON, this->TrayIconUseGameIcon, allocator);
            return obj;
        }

        LFXE_API MainConfigFile::WValue MainConfigFile::SerializeDevices(WDocument::AllocatorType& allocator) {
            WValue obj(kObjectType);

            // Enabled devices
            WValue objEnabledDevices(kObjectType);
            map<wstring, bool> devices = map<wstring, bool>(this->EnabledDevices);
            auto deviceManager = this->GetManager()->GetLightFXExtender()->GetDeviceManager();
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                devices[device->GetDeviceName()] = device->IsEnabled();
            }
            for (auto device : devices) {
                objEnabledDevices.AddMember(WValue(device.first.c_str(), allocator).Move(), WValue(device.second), allocator);
            }
            obj.AddMember(CONF_DEVICES_ENABLED, objEnabledDevices, allocator);

            // LightFX
            WValue objLightfx(kObjectType);

            // LightFX Alienware DLL name
            objLightfx.AddMember(CONF_DEVICES_LIGHTFX_ALIENWAREDLL, this->MakeJsonWString(this->AlienwareDllName, allocator), allocator);
            objLightfx.AddMember(CONF_DEVICES_LIGHTFX_ALIENWAREBACKUPDLL, this->MakeJsonWString(this->AlienwareBackupDllName, allocator), allocator);
            obj.AddMember(CONF_DEVICES_LIGHTFX, objLightfx, allocator);

            // Lightpack
            WValue objLightpack(kObjectType);

            // Lightpack API
            WValue objLightpackApi(kObjectType);
            objLightpackApi.AddMember(CONF_DEVICES_LIGHTPACK_API_HOST, this->MakeJsonWString(this->LightpackHost, allocator), allocator);
            objLightpackApi.AddMember(CONF_DEVICES_LIGHTPACK_API_PORT, this->MakeJsonWString(this->LightpackPort, allocator), allocator);
            objLightpackApi.AddMember(CONF_DEVICES_LIGHTPACK_API_KEY, this->MakeJsonWString(this->LightpackKey, allocator), allocator);
            objLightpack.AddMember(CONF_DEVICES_LIGHTPACK_API, objLightpackApi, allocator);

            obj.AddMember(CONF_DEVICES_LIGHTPACK, objLightpack, allocator);

            // Logitech
            WValue objLogitech(kObjectType);

            // Logitech color range
            WValue objLogitechColorRange(kObjectType);
            objLogitechColorRange.AddMember(CONF_DEVICES_LOGITECH_COLORRANGE_OUTMIN, this->LogitechColorRangeOutMin, allocator);
            objLogitechColorRange.AddMember(CONF_DEVICES_LOGITECH_COLORRANGE_OUTMAX, this->LogitechColorRangeOutMax, allocator);
            objLogitechColorRange.AddMember(CONF_DEVICES_LOGITECH_COLORRANGE_INMIN, this->LogitechColorRangeInMin, allocator);
            objLogitechColorRange.AddMember(CONF_DEVICES_LOGITECH_COLORRANGE_INMAX, this->LogitechColorRangeInMax, allocator);
            objLogitech.AddMember(CONF_DEVICES_LOGITECH_COLORRANGE, objLogitechColorRange, allocator);
			objLogitech.AddMember(CONF_DEVICES_LOGITECH_RESTORELIGHTSONNULL, this->LogitechRestoreLightsOnNullEnabled, allocator);
            objLogitech.AddMember(CONF_DEVICES_LOGITECH_G110WORKAROUND, this->LogitechG110WorkaroundEnabled, allocator);

            obj.AddMember(CONF_DEVICES_LOGITECH, objLogitech, allocator);

			// Logitech
			WValue objCorsair(kObjectType);

			// Logitech color range
			WValue objCorsairColorRange(kObjectType);
			objCorsairColorRange.AddMember(CONF_DEVICES_CORSAIR_COLORRANGE_OUTMIN, this->CorsairColorRangeOutMin, allocator);
			objCorsairColorRange.AddMember(CONF_DEVICES_CORSAIR_COLORRANGE_OUTMAX, this->CorsairColorRangeOutMax, allocator);
			objCorsairColorRange.AddMember(CONF_DEVICES_CORSAIR_COLORRANGE_INMIN, this->CorsairColorRangeInMin, allocator);
			objCorsairColorRange.AddMember(CONF_DEVICES_CORSAIR_COLORRANGE_INMAX, this->CorsairColorRangeInMax, allocator);
			objCorsair.AddMember(CONF_DEVICES_CORSAIR_COLORRANGE, objCorsairColorRange, allocator);

			obj.AddMember(CONF_DEVICES_CORSAIR, objCorsair, allocator);

            return obj;
        }

        LFXE_API MainConfigFile::WValue MainConfigFile::SerializeGames(WDocument::AllocatorType& allocator) {
            WValue obj(kObjectType);

            // Guild Wars 2
            WValue objGuildwars2(kObjectType);
            objGuildwars2.AddMember(CONF_GAMES_GUILDWARS2_TEAMCOLORENABLED, this->GuildWars2TeamColorEnabled, allocator);
            objGuildwars2.AddMember(CONF_GAMES_GUILDWARS2_TEAMCOLORANIMATION, this->MakeJsonWString(this->GuildWars2TeamColorAnimation, allocator), allocator);
            obj.AddMember(CONF_GAMES_GUILDWARS2, objGuildwars2, allocator);

            return obj;
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

            // Timeline update interval
            if (this->doc.HasMember(CONF_TIMELINE_UPDATE_INTERVAL) && this->doc[CONF_TIMELINE_UPDATE_INTERVAL].IsInt()) {
                this->TimelineUpdateInterval = this->doc[CONF_TIMELINE_UPDATE_INTERVAL].GetInt();
            }

            this->DeserializeTrayIcon(this->doc);
            this->DeserializeDevices(this->doc);
            this->DeserializeGames(this->doc);
        }

        LFXE_API void MainConfigFile::DeserializeTrayIcon(MainConfigFile::WDocument& doc) {
            // Tray icon
            if (doc.HasMember(CONF_TRAYICON) && doc[CONF_TRAYICON].IsObject()) {
                const WValue& objTrayIcon = doc[CONF_TRAYICON];
                if (objTrayIcon.HasMember(CONF_TRAYICON_ENABLED) && objTrayIcon[CONF_TRAYICON_ENABLED].IsBool()) {
                    this->TrayIconEnabled = objTrayIcon[CONF_TRAYICON_ENABLED].GetBool();
                }
                if (objTrayIcon.HasMember(CONF_TRAYICON_USEGAMEICON) && objTrayIcon[CONF_TRAYICON_USEGAMEICON].IsBool()) {
                    this->TrayIconUseGameIcon = objTrayIcon[CONF_TRAYICON_USEGAMEICON].GetBool();
                }
            }
        }

        LFXE_API void MainConfigFile::DeserializeDevices(MainConfigFile::WDocument& doc) {
            if (doc.HasMember(CONF_DEVICES) && doc[CONF_DEVICES].IsObject()) {
                const WValue& objDevices = doc[CONF_DEVICES];

                // Enabled devices
                if (objDevices.HasMember(CONF_DEVICES_ENABLED) && objDevices[CONF_DEVICES_ENABLED].IsObject()) {
                    const WValue& objDevicesEnabled = objDevices[CONF_DEVICES_ENABLED];
                    for (auto itr = objDevicesEnabled.MemberBegin(); itr != objDevicesEnabled.MemberEnd(); ++itr) {
                        if (itr->value.IsBool()) {
                            wstring deviceName = itr->name.GetString();
                            bool deviceEnabled = itr->value.GetBool();
                            this->EnabledDevices[deviceName] = deviceEnabled;
                        }
                    }
                }

                // LightFX
                if (objDevices.HasMember(CONF_DEVICES_LIGHTFX) && objDevices[CONF_DEVICES_LIGHTFX].IsObject()) {
                    const WValue& objLightfx = objDevices[CONF_DEVICES_LIGHTFX];
                    if (objLightfx.HasMember(CONF_DEVICES_LIGHTFX_ALIENWAREDLL) && objLightfx[CONF_DEVICES_LIGHTFX_ALIENWAREDLL].IsString()) {
                        this->AlienwareDllName = objLightfx[CONF_DEVICES_LIGHTFX_ALIENWAREDLL].GetString();
                    }
                    if (objLightfx.HasMember(CONF_DEVICES_LIGHTFX_ALIENWAREBACKUPDLL) && objLightfx[CONF_DEVICES_LIGHTFX_ALIENWAREBACKUPDLL].IsString()) {
                        this->AlienwareBackupDllName = objLightfx[CONF_DEVICES_LIGHTFX_ALIENWAREBACKUPDLL].GetString();
                    }
                }

                // Lightpack
                if (objDevices.HasMember(CONF_DEVICES_LIGHTPACK) && objDevices[CONF_DEVICES_LOGITECH].IsObject()) {
                    const WValue& objLightpack = objDevices[CONF_DEVICES_LIGHTPACK];
                    // Lightpack API
                    if (objLightpack.HasMember(CONF_DEVICES_LIGHTPACK_API) && objLightpack[CONF_DEVICES_LIGHTPACK_API].IsObject()) {
                        const WValue& lightpackApi = objLightpack[CONF_DEVICES_LIGHTPACK_API];
                        if (lightpackApi.HasMember(CONF_DEVICES_LIGHTPACK_API_HOST) && lightpackApi[CONF_DEVICES_LIGHTPACK_API_HOST].IsString()) {
                            this->LightpackHost = lightpackApi[CONF_DEVICES_LIGHTPACK_API_HOST].GetString();
                        }
                        if (lightpackApi.HasMember(CONF_DEVICES_LIGHTPACK_API_PORT) && lightpackApi[CONF_DEVICES_LIGHTPACK_API_PORT].IsString()) {
                            this->LightpackPort = lightpackApi[CONF_DEVICES_LIGHTPACK_API_PORT].GetString();
                        }
                        if (lightpackApi.HasMember(CONF_DEVICES_LIGHTPACK_API_KEY) && lightpackApi[CONF_DEVICES_LIGHTPACK_API_KEY].IsString()) {
                            this->LightpackKey = lightpackApi[CONF_DEVICES_LIGHTPACK_API_KEY].GetString();
                        }
                    }
                }

                // Logitech
                if (objDevices.HasMember(CONF_DEVICES_LOGITECH) && objDevices[CONF_DEVICES_LOGITECH].IsObject()) {
                    const WValue& objLogitech = objDevices[CONF_DEVICES_LOGITECH];
                    // Logitech color range
                    if (objLogitech.HasMember(CONF_DEVICES_LOGITECH_COLORRANGE) && objLogitech[CONF_DEVICES_LOGITECH_COLORRANGE].IsObject()) {
                        const WValue& colorRange = objLogitech[CONF_DEVICES_LOGITECH_COLORRANGE];
                        if (colorRange.HasMember(CONF_DEVICES_LOGITECH_COLORRANGE_OUTMIN) && colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_OUTMIN].IsInt()) {
                            this->LogitechColorRangeOutMin = colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_OUTMIN].GetInt();
                        }
                        if (colorRange.HasMember(CONF_DEVICES_LOGITECH_COLORRANGE_OUTMAX) && colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_OUTMAX].IsInt()) {
                            this->LogitechColorRangeOutMax = colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_OUTMAX].GetInt();
                        }
                        if (colorRange.HasMember(CONF_DEVICES_LOGITECH_COLORRANGE_INMIN) && colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_INMIN].IsInt()) {
                            this->LogitechColorRangeInMin = colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_INMIN].GetInt();
                        }
                        if (colorRange.HasMember(CONF_DEVICES_LOGITECH_COLORRANGE_INMAX) && colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_INMAX].IsInt()) {
                            this->LogitechColorRangeInMax = colorRange[CONF_DEVICES_LOGITECH_COLORRANGE_INMAX].GetInt();
                        }
                    }
                    if (objLogitech.HasMember(CONF_DEVICES_LOGITECH_G110WORKAROUND) && objLogitech[CONF_DEVICES_LOGITECH_G110WORKAROUND].IsBool()) {
                        this->LogitechG110WorkaroundEnabled = objLogitech[CONF_DEVICES_LOGITECH_G110WORKAROUND].GetBool();
                    }
					if (objLogitech.HasMember(CONF_DEVICES_LOGITECH_RESTORELIGHTSONNULL) && objLogitech[CONF_DEVICES_LOGITECH_RESTORELIGHTSONNULL].IsBool()) {
						this->LogitechRestoreLightsOnNullEnabled = objLogitech[CONF_DEVICES_LOGITECH_RESTORELIGHTSONNULL].GetBool();
					}
                }

				// Corsair
				if (objDevices.HasMember(CONF_DEVICES_CORSAIR) && objDevices[CONF_DEVICES_CORSAIR].IsObject()) {
					const WValue& objCorsair = objDevices[CONF_DEVICES_CORSAIR];
					// Corsair color range
					if (objCorsair.HasMember(CONF_DEVICES_CORSAIR_COLORRANGE) && objCorsair[CONF_DEVICES_CORSAIR_COLORRANGE].IsObject()) {
						const WValue& colorRange = objCorsair[CONF_DEVICES_CORSAIR_COLORRANGE];
						if (colorRange.HasMember(CONF_DEVICES_CORSAIR_COLORRANGE_OUTMIN) && colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_OUTMIN].IsInt()) {
							this->CorsairColorRangeOutMin = colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_OUTMIN].GetInt();
						}
						if (colorRange.HasMember(CONF_DEVICES_CORSAIR_COLORRANGE_OUTMAX) && colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_OUTMAX].IsInt()) {
							this->CorsairColorRangeOutMax = colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_OUTMAX].GetInt();
						}
						if (colorRange.HasMember(CONF_DEVICES_CORSAIR_COLORRANGE_INMIN) && colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_INMIN].IsInt()) {
							this->CorsairColorRangeInMin = colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_INMIN].GetInt();
						}
						if (colorRange.HasMember(CONF_DEVICES_CORSAIR_COLORRANGE_INMAX) && colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_INMAX].IsInt()) {
							this->CorsairColorRangeInMax = colorRange[CONF_DEVICES_CORSAIR_COLORRANGE_INMAX].GetInt();
						}
					}
				}
            }
        }

        LFXE_API void MainConfigFile::DeserializeGames(MainConfigFile::WDocument& doc) {
            if (doc.HasMember(CONF_GAMES) && doc[CONF_GAMES].IsObject()) {
                const WValue& objGames = doc[CONF_GAMES];

                // Guild Wars 2
                if (objGames.HasMember(CONF_GAMES_GUILDWARS2) && objGames[CONF_GAMES_GUILDWARS2].IsObject()) {
                    const WValue& gw2 = objGames[CONF_GAMES_GUILDWARS2];
                    if (gw2.HasMember(CONF_GAMES_GUILDWARS2_TEAMCOLORENABLED) && gw2[CONF_GAMES_GUILDWARS2_TEAMCOLORENABLED].IsBool()) {
                        this->GuildWars2TeamColorEnabled = gw2[CONF_GAMES_GUILDWARS2_TEAMCOLORENABLED].GetBool();
                    }
                    if (gw2.HasMember(CONF_GAMES_GUILDWARS2_TEAMCOLORANIMATION) && gw2[CONF_GAMES_GUILDWARS2_TEAMCOLORANIMATION].IsString()) {
                        this->GuildWars2TeamColorAnimation = gw2[CONF_GAMES_GUILDWARS2_TEAMCOLORANIMATION].GetString();
                    }
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

#include "Config.h"

// Standard includes
#include <iostream>
#include <fstream>
#include <codecvt>
#include <sstream>

// Windows includes
#include <ShlObj.h>

// 3rd party includes
#include "rapidjson/prettywriter.h"

// Project includes
#include "Utils/FileIO.h"
#include "Utils/Log.h"


#define APPDATA_CONFIGFILE L"settings.json"

#define CONF_DEVICESTATES L"EnabledDevices"

#define CONF_LOGITECHCOLORANGE L"LogitechColorRange"
#define CONF_LOGITECHCOLORRANGE_OUTMIN L"AlienFxMin"
#define CONF_LOGITECHCOLORRANGE_OUTMAX L"AlienFxMax"
#define CONF_LOGITECHCOLORRANGE_INMIN L"LogitechMin"
#define CONF_LOGITECHCOLORRANGE_INMAX L"LogitechMax"

#define CONF_LIGHTPACKAPI L"LightpackAPI"
#define CONF_LIGHTPACKHOST L"Hostname"
#define CONF_LIGHTPACKPORT L"PortNumber"
#define CONF_LIGHTPACKKEY L"Key"

using namespace std;
using namespace rapidjson;
using namespace lightfx::utils;

namespace lightfx {

    bool Config::Load() {
        this->SetDefault();
        try {
            wstring configPath;
            if (GetRoamingAppDataFolder(configPath)) {
                configPath = configPath + L"/" + APPDATA_FOLDER;
                if (DirExists(configPath)) {
                    configPath = configPath + L"/" + APPDATA_CONFIGFILE;

                    wifstream configStream;
                    configStream.imbue(locale(configStream.getloc(), new codecvt_utf8<wchar_t>));
                    configStream.open(configPath, wios::in | wios::binary);

                    wstringstream stream;
                    stream << configStream.rdbuf();

                    wstring json = stream.str();
                    this->doc.Parse<0>(json.c_str());
                    configStream.close();
                    this->Parse();
                }
                return true;
            }
        } catch (...) {
            Log(L"Failed to load configuration");
        }
        return false;
    }

    bool Config::Save() {
        try {
            this->Commit();

            wstring configPath;
            if (GetRoamingAppDataFolder(configPath)) {
                configPath = configPath + L"/" + APPDATA_FOLDER;
                if (!DirExists(configPath)) {
                    if (CreateDirectoryW(configPath.c_str(), NULL) == FALSE) {
                        Log(L"Failed to create folder in %APPDATA%");
                        LogLastError();
                        return false;
                    }
                }
                configPath = configPath + L"/" + APPDATA_CONFIGFILE;

                GenericStringBuffer<UTF16<>> buffer;
                PrettyWriter<GenericStringBuffer<UTF16<>>, UTF16<>, UTF16<>> writer(buffer);
                this->doc.Accept(writer);
                
                wstring json = buffer.GetString();
                wofstream configStream;
                configStream.imbue(locale(configStream.getloc(), new codecvt_utf8<wchar_t>));
                configStream.open(configPath, wios::out | wios::binary);
                configStream << json;
                configStream.close();

                return true;
            }
        } catch (...) {
            Log(L"Failed to save configuration");
        }
        return false;
    }

    void Config::SetDefault() {
        this->LogitechColorRangeOutMin = 0;
        this->LogitechColorRangeOutMax = 255;
        this->LogitechColorRangeInMin = 0;
        this->LogitechColorRangeInMax = 100;
    }

    void Config::Parse() {
        if (!this->doc.IsObject()) {
            return;
        }

        if (this->doc.HasMember(CONF_DEVICESTATES) && this->doc[CONF_DEVICESTATES].IsObject()) {
            const GenericValue<UTF16<>>& deviceStates = this->doc[CONF_DEVICESTATES];
            for (auto itr = deviceStates.MemberBegin(); itr != deviceStates.MemberEnd(); ++itr) {
                if (itr->value.IsBool()) {
                    wstring deviceName = itr->name.GetString();
                    bool deviceEnabled = itr->value.GetBool();
                    this->DeviceStates[deviceName] = deviceEnabled;
                }
            }
        }

        if (this->doc.HasMember(CONF_LOGITECHCOLORANGE) && this->doc[CONF_LOGITECHCOLORANGE].IsObject()) {
            const GenericValue<UTF16<>>& colorRange = this->doc[CONF_LOGITECHCOLORANGE];
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

        if (this->doc.HasMember(CONF_LIGHTPACKAPI) && this->doc[CONF_LIGHTPACKAPI].IsObject()) {
            const GenericValue<UTF16<>>& api = this->doc[CONF_LIGHTPACKAPI];
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
    }

    void Config::Commit() {
        GenericDocument<UTF16<>>::AllocatorType& allocator = this->doc.GetAllocator();
        this->doc.SetObject();

        GenericValue<UTF16<>> deviceStates(kObjectType);
        for (auto itr : this->DeviceStates) {
            deviceStates.AddMember(GenericValue<UTF16<>>(itr.first.c_str(), allocator).Move(), GenericValue<UTF16<>>(itr.second), allocator);
        }
        this->doc.AddMember(CONF_DEVICESTATES, deviceStates, allocator);

        GenericValue<UTF16<>> logiRange(kObjectType);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMIN, this->LogitechColorRangeOutMin, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMAX, this->LogitechColorRangeOutMax, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_INMIN, this->LogitechColorRangeInMin, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_INMAX, this->LogitechColorRangeInMax, allocator);
        this->doc.AddMember(CONF_LOGITECHCOLORANGE, logiRange, allocator);

        GenericValue<UTF16<>> lightpackApi(kObjectType);
        GenericValue<UTF16<>> host(kStringType);
        host.SetString(this->LightpackHost.c_str(), allocator);
        lightpackApi.AddMember(CONF_LIGHTPACKHOST, host, allocator);
        GenericValue<UTF16<>> port(kStringType);
        port.SetString(this->LightpackPort.c_str(), allocator);
        lightpackApi.AddMember(CONF_LIGHTPACKPORT, port, allocator);
        GenericValue<UTF16<>> key(kStringType);
        key.SetString(this->LightpackKey.c_str(), allocator);
        lightpackApi.AddMember(CONF_LIGHTPACKKEY, key, allocator);
        this->doc.AddMember(CONF_LIGHTPACKAPI, lightpackApi, allocator);
    }

}

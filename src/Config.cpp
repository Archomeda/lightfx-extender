#include "Config.h"

// Windows includes
#include <ShlObj.h>

// 3rd party includes
#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"
#include "rapidjson\prettywriter.h"

// Project includes
#include "Utils/FileIO.h"
#include "Utils/Log.h"


#define APPDATA_CONFIGFILE L"settings.json"

#define CONF_DEVICESTATES "EnabledDevices"

#define CONF_LOGITECHCOLORANGE "LogitechColorRange"
#define CONF_LOGITECHCOLORRANGE_OUTMIN "AlienFxMin"
#define CONF_LOGITECHCOLORRANGE_OUTMAX "AlienFxMax"
#define CONF_LOGITECHCOLORRANGE_INMIN "LogitechMin"
#define CONF_LOGITECHCOLORRANGE_INMAX "LogitechMax"

#define CONF_LIGHTPACKAPI "LightpackAPI"
#define CONF_LIGHTPACKHOST "Hostname"
#define CONF_LIGHTPACKPORT "PortNumber"
#define CONF_LIGHTPACKKEY "Key"

using namespace std;
using namespace rapidjson;
using namespace lightfx::utils;

namespace lightfx {

    bool Config::Load() {
        this->SetDefault();
        try {
            // TODO: Find a better way to read JSON from a file (with no manually managed buffer)
            wstring configPath;
            if (GetRoamingAppDataFolder(configPath)) {
                configPath = configPath + L"/" + APPDATA_FOLDER;
                if (DirExists(configPath)) {
                    configPath = configPath + L"/" + APPDATA_CONFIGFILE;
                    FILE* pFile;
                    if (_wfopen_s(&pFile, configPath.c_str(), L"rb") == 0) {
                        char buffer[65536];
                        FileReadStream frs(pFile, buffer, sizeof(buffer));
                        this->doc.ParseStream<0, UTF8<>, FileReadStream>(frs);
                        this->Parse();
                        fclose(pFile);
                    }
                    pFile = nullptr;
                }
                return true;
            }
        } catch (...) {
            Log("Failed to load configuration");
        }
        return false;
    }

    bool Config::Save() {
        try {
            this->Commit();

            // TODO: Find a better way to write JSON to a file (with no manually managed buffer)
            wstring configPath;
            if (GetRoamingAppDataFolder(configPath)) {
                configPath = configPath + L"/" + APPDATA_FOLDER;
                if (!DirExists(configPath)) {
                    if (CreateDirectoryW(configPath.c_str(), NULL) == FALSE) {
                        Log("Failed to create folder in %APPDATA%");
                        LogLastError();
                        return false;
                    }
                }
                configPath = configPath + L"/" + APPDATA_CONFIGFILE;
                FILE* pFile;
                if (_wfopen_s(&pFile, configPath.c_str(), L"wb") == 0) {
                    char buffer[65536];
                    FileWriteStream fws(pFile, buffer, sizeof(buffer));
                    PrettyWriter<FileWriteStream> writer(fws);
                    this->doc.Accept(writer);
                    fclose(pFile);
                }
                pFile = nullptr;
                return true;
            }
        } catch (...) {
            Log("Failed to save configuration");
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
            const Value& deviceStates = this->doc[CONF_DEVICESTATES];
            for (auto itr = deviceStates.MemberBegin(); itr != deviceStates.MemberEnd(); ++itr) {
                if (itr->value.IsBool()) {
                    string deviceName = itr->name.GetString();
                    bool deviceEnabled = itr->value.GetBool();
                    this->DeviceStates[deviceName] = deviceEnabled;
                }
            }
        }

        if (this->doc.HasMember(CONF_LOGITECHCOLORANGE) && this->doc[CONF_LOGITECHCOLORANGE].IsObject()) {
            const Value& colorRange = this->doc[CONF_LOGITECHCOLORANGE];
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
            const Value& api = this->doc[CONF_LIGHTPACKAPI];
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
        Document::AllocatorType& allocator = this->doc.GetAllocator();
        this->doc.SetObject();

        Value deviceStates(kObjectType);
        for (auto itr : this->DeviceStates) {
            deviceStates.AddMember(Value(itr.first.c_str(), allocator).Move(), Value(itr.second), allocator);
        }
        this->doc.AddMember(CONF_DEVICESTATES, deviceStates, allocator);

        Value logiRange(kObjectType);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMIN, this->LogitechColorRangeOutMin, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMAX, this->LogitechColorRangeOutMax, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_INMIN, this->LogitechColorRangeInMin, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_INMAX, this->LogitechColorRangeInMax, allocator);
        this->doc.AddMember(CONF_LOGITECHCOLORANGE, logiRange, allocator);

        Value lightpackApi(kObjectType);
        Value host(kStringType);
        host.SetString(this->LightpackHost.c_str(), allocator);
        lightpackApi.AddMember(CONF_LIGHTPACKHOST, host, allocator);
        Value port(kStringType);
        port.SetString(this->LightpackPort.c_str(), allocator);
        lightpackApi.AddMember(CONF_LIGHTPACKPORT, port, allocator);
        Value key(kStringType);
        key.SetString(this->LightpackKey.c_str(), allocator);
        lightpackApi.AddMember(CONF_LIGHTPACKKEY, key, allocator);
        this->doc.AddMember(CONF_LIGHTPACKAPI, lightpackApi, allocator);
    }

}

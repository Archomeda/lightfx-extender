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

#define CONF_LOGITECHENABLED "LogitechEnabled"
#define CONF_LOGITECHCOLORANGE "LogitechColorRange"
#define CONF_LOGITECHCOLORRANGE_OUTMIN "AlienFxMin"
#define CONF_LOGITECHCOLORRANGE_OUTMAX "AlienFxMax"
#define CONF_LOGITECHCOLORRANGE_INMIN "LogitechMin"
#define CONF_LOGITECHCOLORRANGE_INMAX "LogitechMax"

#define CONF_LIGHTPACKENABLED "LightpackEnabled"
#define CONF_LIGHTPACKAPI "LightpackAPI"
#define CONF_LIGHTPACKHOST "Hostname"
#define CONF_LIGHTPACKPORT "PortNumber"
#define CONF_LIGHTPACKKEY "Key"

#define CONF_MMFENABLED "MemoryMappedFileEnabled"
#define CONF_MMFDEVICES "MemoryMappedFileDevices"
#define CONF_MMFDEVICE_DESC "Description"
#define CONF_MMFDEVICE_TYPE "Type"
#define CONF_MMFDEVICE_LIGHTS "Lights"
#define CONF_MMFDEVICE_LIGHT_DESC "Description"
#define CONF_MMFDEVICE_LIGHT_POSX "PosX"
#define CONF_MMFDEVICE_LIGHT_POSY "PosY"
#define CONF_MMFDEVICE_LIGHT_POSZ "PosZ"

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
        this->LogitechEnabled = true;
        this->LightpackEnabled = false;
        this->MemoryMappedFileEnabled = false;
        this->LogitechColorRangeOutMin = 0;
        this->LogitechColorRangeOutMax = 255;
        this->LogitechColorRangeInMin = 0;
        this->LogitechColorRangeInMax = 100;

        // Default MMF device
        DeviceData device;
        device.Description = "AlienFX Keyboard Emulator";
        device.Type = LFX_DEVTYPE_KEYBOARD;
        DeviceLightData light;
        light.Description = "";
        light.Position = LFX_POSITION();
        device.Lights.push_back(light);
        this->MemoryMappedFileDevices.push_back(device);
    }

    void Config::Parse() {
        if (!this->doc.IsObject()) {
            return;
        }

        if (this->doc.HasMember(CONF_LOGITECHENABLED) && this->doc[CONF_LOGITECHENABLED].IsBool()) {
            this->LogitechEnabled = this->doc[CONF_LOGITECHENABLED].GetBool();
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

        if (this->doc.HasMember(CONF_LIGHTPACKENABLED) && this->doc[CONF_LIGHTPACKENABLED].IsBool()) {
            this->LightpackEnabled = this->doc[CONF_LIGHTPACKENABLED].GetBool();
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

        if (this->doc.HasMember(CONF_MMFENABLED) && this->doc[CONF_MMFENABLED].IsBool()) {
            this->MemoryMappedFileEnabled = this->doc[CONF_MMFENABLED].GetBool();
        }

        if (this->doc.HasMember(CONF_MMFDEVICES) && this->doc[CONF_MMFDEVICES].IsArray()) {
            this->MemoryMappedFileDevices.clear();
            for (SizeType i = 0; i < this->doc[CONF_MMFDEVICES].Size(); i++) {
                const Value& dev = this->doc[CONF_MMFDEVICES][i];
                DeviceData device;

                if (dev.HasMember(CONF_MMFDEVICE_DESC) && dev[CONF_MMFDEVICE_DESC].IsString()) {
                    device.Description = dev[CONF_MMFDEVICE_DESC].GetString();
                }
                if (dev.HasMember(CONF_MMFDEVICE_TYPE) && dev[CONF_MMFDEVICE_TYPE].IsUint()) {
                    device.Type = dev[CONF_MMFDEVICE_TYPE].GetUint();
                }
                if (dev.HasMember(CONF_MMFDEVICE_LIGHTS) && dev[CONF_MMFDEVICE_LIGHTS].IsArray()) {
                    for (SizeType j = 0; j < dev[CONF_MMFDEVICE_LIGHTS].Size(); j++) {
                        const Value& lght = dev[CONF_MMFDEVICE_LIGHTS][j];
                        DeviceLightData light;
                        if (lght.HasMember(CONF_MMFDEVICE_LIGHT_DESC) && lght[CONF_MMFDEVICE_LIGHT_DESC].IsString()) {
                            light.Description = lght[CONF_MMFDEVICE_LIGHT_DESC].GetString();
                        }
                        LFX_POSITION pos;
                        if (lght.HasMember(CONF_MMFDEVICE_LIGHT_POSX) && lght[CONF_MMFDEVICE_LIGHT_POSX].IsUint()) {
                            pos.x = lght[CONF_MMFDEVICE_LIGHT_POSX].GetUint();
                        }
                        if (lght.HasMember(CONF_MMFDEVICE_LIGHT_POSY) && lght[CONF_MMFDEVICE_LIGHT_POSY].IsUint()) {
                            pos.y = lght[CONF_MMFDEVICE_LIGHT_POSY].GetUint();
                        }
                        if (lght.HasMember(CONF_MMFDEVICE_LIGHT_POSZ) && lght[CONF_MMFDEVICE_LIGHT_POSZ].IsUint()) {
                            pos.z = lght[CONF_MMFDEVICE_LIGHT_POSZ].GetUint();
                        }
                        light.Position = pos;
                        device.Lights.push_back(light);
                    }
                    this->MemoryMappedFileDevices.push_back(device);
                }
            }
        }
    }

    void Config::Commit() {
        Document::AllocatorType& allocator = this->doc.GetAllocator();

        this->doc.SetObject();
        this->doc.AddMember(CONF_LOGITECHENABLED, this->LogitechEnabled, allocator);
        Value logiRange(kObjectType);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMIN, this->LogitechColorRangeOutMin, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_OUTMAX, this->LogitechColorRangeOutMax, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_INMIN, this->LogitechColorRangeInMin, allocator);
        logiRange.AddMember(CONF_LOGITECHCOLORRANGE_INMAX, this->LogitechColorRangeInMax, allocator);
        this->doc.AddMember(CONF_LOGITECHCOLORANGE, logiRange, allocator);

        this->doc.AddMember(CONF_LIGHTPACKENABLED, this->LightpackEnabled, allocator);
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

        this->doc.AddMember(CONF_MMFENABLED, this->MemoryMappedFileEnabled, allocator);
        Value devs(kArrayType);
        for (size_t i = 0; i < this->MemoryMappedFileDevices.size(); i++) {
            DeviceData device = this->MemoryMappedFileDevices[i];
            Value dev(kObjectType);
            Value desc(kStringType);
            desc.SetString(device.Description.c_str(), allocator);
            dev.AddMember(CONF_MMFDEVICE_DESC, desc, allocator);
            dev.AddMember(CONF_MMFDEVICE_TYPE, device.Type, allocator);
            Value lights(kArrayType);
            for (size_t j = 0; j < device.Lights.size(); j++) {
                DeviceLightData light = device.Lights[j];
                Value lght(kObjectType);
                Value desc2(kStringType);
                desc2.SetString(light.Description.c_str(), allocator);
                lght.AddMember(CONF_MMFDEVICE_LIGHT_DESC, desc2, allocator);
                lght.AddMember(CONF_MMFDEVICE_LIGHT_POSX, light.Position.x, allocator);
                lght.AddMember(CONF_MMFDEVICE_LIGHT_POSY, light.Position.y, allocator);
                lght.AddMember(CONF_MMFDEVICE_LIGHT_POSZ, light.Position.z, allocator);
                lights.PushBack(lght, allocator);
            }
            dev.AddMember(CONF_MMFDEVICE_LIGHTS, lights, allocator);
            devs.PushBack(dev, allocator);
        }
        this->doc.AddMember(CONF_MMFDEVICES, devs, allocator);
    }

}

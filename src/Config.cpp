#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"
#include "rapidjson\prettywriter.h"
#include "Config.h"

#define CONFIGFILE "LightFX.txt"

#define CONF_LOGITECHENABLED "LogitechEnabled"
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

namespace lightfx {

    void Config::Load() {
        this->SetDefault();
        try {
            // TODO: Find a better way to read JSON from a file (with no manually managed buffer)
            FILE* pFile;
            if (fopen_s(&pFile, CONFIGFILE, "r") == 0) {
                char buffer[65536];
                FileReadStream frs(pFile, buffer, sizeof(buffer));
                this->doc.ParseStream<0, UTF8<>, FileReadStream>(frs);
                this->Parse();
                fclose(pFile);
            }
            pFile = nullptr;
        } catch (...) {}
    }

    void Config::Save() {
        try {
            this->Commit();

            // TODO: Find a better way to write JSON to a file (with no manually managed buffer)
            FILE* pFile;
            if (fopen_s(&pFile, CONFIGFILE, "w") == 0) {
                char buffer[65536];
                FileWriteStream fws(pFile, buffer, sizeof(buffer));
                PrettyWriter<FileWriteStream> writer(fws);
                this->doc.Accept(writer);
                fclose(pFile);
            }
            pFile = nullptr;
        } catch (...) {}
    }

    void Config::SetDefault() {
        this->LogitechEnabled = true;
        this->MemoryMappedFileEnabled = false;

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

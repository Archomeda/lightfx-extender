#pragma once
#include <vector>
#include "rapidjson\document.h"
#include "Structs.h"

namespace lightfx {

    class Config {

    public:
        bool Load();
        bool Save();

        bool LogitechEnabled = true;
        int LogitechColorRangeOutMin = 0;
        int LogitechColorRangeOutMax = 255;
        int LogitechColorRangeInMin = 0;
        int LogitechColorRangeInMax = 100;

        bool LightpackEnabled = false;
        std::string LightpackHost = "127.0.0.1";
        std::string LightpackPort = "3636";
        std::string LightpackKey = "";

        bool MemoryMappedFileEnabled = false;
        std::vector<DeviceData> MemoryMappedFileDevices = {};

    protected:
        void SetDefault();
        void Parse();
        void Commit();

    private:
        rapidjson::Document doc;

    };

}

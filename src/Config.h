#pragma once
#include <vector>
#include "rapidjson\document.h"
#include "Structs.h"

namespace lightfx {

    class Config {

    public:
        void Load();
        void Save();

        bool LogitechEnabled = true;
        int LogitechColorRangeOutMin = 0;
        int LogitechColorRangeOutMax = 255;
        int LogitechColorRangeInMin = 0;
        int LogitechColorRangeInMax = 100;
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

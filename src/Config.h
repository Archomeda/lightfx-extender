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

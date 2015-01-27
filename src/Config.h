#pragma once

// Standard includes
#include <vector>
#include <map>

// 3rd party includes
#include "rapidjson/document.h"

// Project includes
#include "Devices/DeviceLight.h"


namespace lightfx {

    class Config {

    public:
        bool Load();
        bool Save();

        std::map<std::string, bool> DeviceStates = {};

        int LogitechColorRangeOutMin = 0;
        int LogitechColorRangeOutMax = 255;
        int LogitechColorRangeInMin = 0;
        int LogitechColorRangeInMax = 100;

        std::string LightpackHost = "127.0.0.1";
        std::string LightpackPort = "3636";
        std::string LightpackKey = "";

    protected:
        void SetDefault();
        void Parse();
        void Commit();

    private:
        rapidjson::Document doc;

    };

}

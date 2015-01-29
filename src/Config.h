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

        std::map<std::wstring, bool> DeviceStates = {};

        int LogitechColorRangeOutMin = 0;
        int LogitechColorRangeOutMax = 255;
        int LogitechColorRangeInMin = 0;
        int LogitechColorRangeInMax = 100;

        std::wstring LightpackHost = L"127.0.0.1";
        std::wstring LightpackPort = L"3636";
        std::wstring LightpackKey = L"";

    protected:
        void SetDefault();
        void Parse();
        void Commit();

    private:
        rapidjson::GenericDocument<rapidjson::UTF16<>> doc;

    };

}

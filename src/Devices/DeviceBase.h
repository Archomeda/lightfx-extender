#pragma once

// Standard includes
#include <string>
#include <vector>

// 3rd party includes
#include "LFXDecl.h"

// Project includes
#include "../Structs.h"


namespace lightfx {
    namespace devices {

        class DeviceBase {

        public:
            DeviceBase();

            bool IsEnabled() { return this->IsEnabled_; }
            bool IsInitialized() { return this->IsInitialized_; }

            virtual bool EnableDevice();
            virtual bool DisableDevice();

            virtual LFX_RESULT Initialize();
            virtual LFX_RESULT Release();
            virtual LFX_RESULT Reset();
            virtual LFX_RESULT Update();

            virtual LFX_RESULT GetDeviceInfo(std::string& description, unsigned char& type) = 0;
            virtual LFX_RESULT GetNumLights(unsigned int& numLights);
            virtual LFX_RESULT GetLightDescription(const unsigned int index, std::string& description);
            virtual LFX_RESULT GetLightLocation(const unsigned int index, LFX_POSITION& location);

            virtual LFX_RESULT GetLightColor(const unsigned int index, LFX_COLOR& color);
            virtual LFX_RESULT SetLightColor(const unsigned int index, const LFX_COLOR& color);
            virtual LFX_RESULT Light(const unsigned int locationMask, const LFX_COLOR& color);

            virtual LFX_RESULT SetLightActionColor(const unsigned int index, const unsigned int actionType, const LFX_COLOR& primaryColor);
            virtual LFX_RESULT SetLightActionColor(const unsigned int index, const unsigned int actionType, const LFX_COLOR& primaryColor, const LFX_COLOR& secondaryColor);
            virtual LFX_RESULT ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryColor);
            virtual LFX_RESULT ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryColor, const LFX_COLOR& secondaryColor);

            virtual LFX_RESULT GetTiming(int& timing);
            virtual LFX_RESULT SetTiming(const int newTiming);

        protected:
            bool IsEnabled_ = false;
            bool IsInitialized_ = false;

            std::vector<DeviceLightData> Lights = {};
            std::vector<LFX_COLOR> CurrentPrimaryColor = {};
            std::vector<LFX_COLOR> NextPrimaryColor = {};
            std::vector<LFX_COLOR> CurrentSecondaryColor = {};
            std::vector<LFX_COLOR> NextSecondaryColor = {};
            std::vector<int> CurrentAction = {};
            std::vector<int> NextAction = {};
            int CurrentTiming = 200;
            int NextTiming = 200;

        };

    }
}

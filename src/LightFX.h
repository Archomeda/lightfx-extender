#pragma once
#include <string>
#include "LFX2.h"

namespace lightfx {


    class LightFX {

    public:
        LightFX();
        ~LightFX();

        LFX_RESULT Initialize();
        LFX_RESULT Release();
        LFX_RESULT Reset();
        LFX_RESULT Update();
        LFX_RESULT UpdateDefault();

        LFX_RESULT GetNumDevices(unsigned int& numDevices);
        LFX_RESULT GetDeviceDescription(const unsigned int devIndex, std::string& devDesc, const unsigned int devDescSize, unsigned char& devType);

        LFX_RESULT GetNumLights(const unsigned int devIndex, unsigned int& numLights);
        LFX_RESULT GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, std::string& lightDesc, const unsigned int devDescSize);
        LFX_RESULT GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, LFX_POSITION& lightLoc);

        LFX_RESULT GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, LFX_COLOR& lightCol);
        LFX_RESULT SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const LFX_COLOR& lightCol);
        LFX_RESULT Light(const unsigned int locationMask, const LFX_COLOR& lightCol);

        LFX_RESULT SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const LFX_COLOR& primaryCol);
        LFX_RESULT SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol);
        LFX_RESULT ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryCol);
        LFX_RESULT ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol);

        LFX_RESULT SetTiming(const int newTiming);
        LFX_RESULT GetVersion(std::string& version, const unsigned int versionSize);

    private:
        bool isInitialized = false;

    };
}

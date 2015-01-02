#include <string>

// Include Logitech LED API
#include <Windows.h> // Windows.h is needed for some declarations inside LogitechLEDLib.h
#include "LogitechLEDLib.h"

#include "LightFX.h"
#include "LinkedMemAPI.h"


using namespace std;

#define NUMDEVICES 2
#define MAXNUMLIGHTS 1
#define LFX_VERSION "2.2.0.0"

namespace lightfx {
    const string        _devDesc[NUMDEVICES]                    = { "AlienFX Keyboard Emulator", "Logitech LED" };
    const unsigned int  _devType[NUMDEVICES]                    = { LFX_DEVTYPE_KEYBOARD, LFX_DEVTYPE_KEYBOARD };
    const unsigned int  _devNumLights[NUMDEVICES]               = { 1, 1 };
    const string        _lightDesc[NUMDEVICES][MAXNUMLIGHTS]    = { { "All" }, { "All" } };
    const unsigned char _lightPosX[NUMDEVICES][MAXNUMLIGHTS]    = { { 0 }, { 0 } };
    const unsigned char _lightPosY[NUMDEVICES][MAXNUMLIGHTS]    = { { 0 }, { 0 } };
    const unsigned char _lightPosZ[NUMDEVICES][MAXNUMLIGHTS]    = { { 0 }, { 0 } };

    bool logiInitialized = false;

    LFX_COLOR currPrimaryColor[NUMDEVICES][MAXNUMLIGHTS];
    LFX_COLOR nextPrimaryColor[NUMDEVICES][MAXNUMLIGHTS];

    LFX_COLOR currSecondaryColor[NUMDEVICES][MAXNUMLIGHTS];
    LFX_COLOR nextSecondaryColor[NUMDEVICES][MAXNUMLIGHTS];

    unsigned int currAction[NUMDEVICES][MAXNUMLIGHTS];
    unsigned int nextAction[NUMDEVICES][MAXNUMLIGHTS];

    int currTiming;
    int nextTiming;


    LightFX::LightFX() {
        //api::init();
    }

    LightFX::~LightFX() {
        //api::dispose();
    }

    LFX_RESULT LightFX::Initialize() {
        if (!this->isInitialized) {
            // Initialize Logitech LED API
            logiInitialized = LogiLedInit();

            // Initialize memory mapped file API
            api::init();
            api::linkedMemHandle->isInitialized = true;
            LFX_POSITION lightPos = LFX_POSITION();
            lightPos.x = _lightPosX[0][0];
            lightPos.y = _lightPosY[0][0];
            lightPos.z = _lightPosZ[0][0];
            api::linkedMemHandle->light.position = lightPos;
            api::linkedMemHandle->tick++;

            // Finalize
            this->isInitialized = true;
            this->Reset();
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::Release() {
        if (this->isInitialized) {
            // Shutdown Logitech LED API
            if (logiInitialized) {
                LogiLedShutdown();
            }

            // Shutdown memory mapped file API
            api::linkedMemHandle->isInitialized = false;
            api::linkedMemHandle->tick++;
            api::dispose();

            // Finalize
            this->isInitialized = false;
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::Reset() {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        for (int i = 0; i < NUMDEVICES; i++) {
            for (int j = 0; j < MAXNUMLIGHTS; j++) {
                currPrimaryColor[i][j] = LFX_COLOR();
                nextPrimaryColor[i][j] = LFX_COLOR();
                currSecondaryColor[i][j] = LFX_COLOR();
                nextSecondaryColor[i][j] = LFX_COLOR();
                currAction[i][j] = LFX_ACTION_COLOR;
                nextAction[i][j] = LFX_ACTION_COLOR;
                currTiming = 200;
                nextTiming = 200;
            }
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::Update() {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        for (int i = 0; i < NUMDEVICES; i++) {
            for (int j = 0; j < MAXNUMLIGHTS; j++) {
                currPrimaryColor[i][j] = LFX_COLOR(nextPrimaryColor[i][j]);
                currSecondaryColor[i][j] = LFX_COLOR(nextSecondaryColor[i][j]);
                currAction[i][j] = nextAction[i][j];
                currTiming = nextTiming;
            }
        }

        // Update Logitech LED API
        if (logiInitialized) {
            float brightness = currPrimaryColor[1][0].brightness / (float)255;
            int logiRed = int(currPrimaryColor[1][0].red / 2.55 * brightness);
            int logiGreen = int(currPrimaryColor[1][0].green / 2.55 * brightness);
            int logiBlue = int(currPrimaryColor[1][0].blue / 2.55 * brightness);
            LogiLedSetLighting(logiRed, logiGreen, logiBlue);
        }

        // Update memory mapped file API
        api::linkedMemHandle->light.primaryColor = LFX_COLOR(currPrimaryColor[0][0]);
        api::linkedMemHandle->light.secondaryColor = LFX_COLOR(currSecondaryColor[0][0]);
        api::linkedMemHandle->light.actionType = currAction[0][0];
        api::linkedMemHandle->timing = currTiming;
        api::linkedMemHandle->tick++;

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::UpdateDefault() {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        return LFX_FAILURE; // Not currently supported
    }

    LFX_RESULT LightFX::GetNumDevices(unsigned int& numDevices) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        numDevices = NUMDEVICES;
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetDeviceDescription(const unsigned int devIndex, string& devDesc, const unsigned int devDescSize, unsigned char& devType) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        string desc = _devDesc[devIndex];
        if (devDescSize < desc.length()) {
            return LFX_ERROR_BUFFSIZE;
        }

        devDesc = desc;
        devType = _devType[devIndex];
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetNumLights(const unsigned int devIndex, unsigned int& numLights) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        numLights = _devNumLights[devIndex];
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, string& lightDesc, const unsigned int devDescSize) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > _devNumLights[devIndex]) {
            return LFX_ERROR_NOLIGHTS;
        }

        string desc = _lightDesc[devIndex][lightIndex];
        if (devDescSize < desc.length()) {
            return LFX_ERROR_BUFFSIZE;
        }

        lightDesc = desc;
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, LFX_POSITION& lightLoc) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > _devNumLights[devIndex]) {
            return LFX_ERROR_NOLIGHTS;
        }

        lightLoc.x = _lightPosX[devIndex][lightIndex];
        lightLoc.y = _lightPosY[devIndex][lightIndex];
        lightLoc.z = _lightPosZ[devIndex][lightIndex];
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, LFX_COLOR& lightCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > _devNumLights[devIndex]) {
            return LFX_ERROR_NOLIGHTS;
        }

        lightCol = LFX_COLOR(currPrimaryColor[devIndex][lightIndex]);
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const LFX_COLOR& lightCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > _devNumLights[devIndex]) {
            return LFX_ERROR_NOLIGHTS;
        }

        nextPrimaryColor[devIndex][lightIndex] = LFX_COLOR(lightCol);
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::Light(const unsigned int locationMask, const LFX_COLOR& lightCol) {
        // locationMask is ignored for now, reserved for possible future use

        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        for (int i = 0; i < NUMDEVICES; i++) {
            for (int j = 0; j < MAXNUMLIGHTS; j++) {
                nextPrimaryColor[i][j] = LFX_COLOR(lightCol);
            }
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const LFX_COLOR& primaryCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > _devNumLights[devIndex]) {
            return LFX_ERROR_NOLIGHTS;
        }

        return this->SetLightActionColor(devIndex, lightIndex, actionType, primaryCol, LFX_COLOR());
    }

    LFX_RESULT LightFX::SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const unsigned int actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) {
        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        if (devIndex > NUMDEVICES) {
            return LFX_ERROR_NODEVS;
        }

        if (lightIndex > _devNumLights[devIndex]) {
            return LFX_ERROR_NOLIGHTS;
        }

        nextAction[devIndex][lightIndex] = actionType;
        nextPrimaryColor[devIndex][lightIndex] = LFX_COLOR(primaryCol);
        if (actionType == LFX_ACTION_MORPH) {
            nextSecondaryColor[devIndex][lightIndex] = LFX_COLOR(secondaryCol);
        }

        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryCol) {
        // locationMask is ignored for now, reserved for possible future use

        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        return this->ActionColor(locationMask, actionType, primaryCol, LFX_COLOR());
    }

    LFX_RESULT LightFX::ActionColor(const unsigned int locationMask, const unsigned int actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) {
        // locationMask is ignored for now, reserved for possible future use

        if (!this->isInitialized) {
            return LFX_ERROR_NOINIT;
        }

        for (int i = 0; i < NUMDEVICES; i++) {
            for (int j = 0; j < MAXNUMLIGHTS; j++) {
                nextAction[i][j] = actionType;
                nextPrimaryColor[i][j] = LFX_COLOR(primaryCol);
                if (actionType == LFX_ACTION_MORPH) {
                    nextSecondaryColor[i][j] = LFX_COLOR(secondaryCol);
                }
            }
        }
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::SetTiming(const int newTiming) {
        nextTiming = newTiming;
        return LFX_SUCCESS;
    }

    LFX_RESULT LightFX::GetVersion(string& version, const unsigned int versionSize) {
        string ver = LFX_VERSION;
        if (versionSize < ver.length()) {
            return LFX_ERROR_BUFFSIZE;
        }

        version = ver;
        return LFX_SUCCESS;
    }

}

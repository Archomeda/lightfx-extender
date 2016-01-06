#pragma once

// Standard includes
#include <string>

// Project includes
#include "LibraryBase.h"
#include "../../Utils/Log.h"

// 3rd party includes
#include "LogitechLEDLib.h"

// API exports
#include "../../Common/ApiExports.h"


namespace lightfx {
    namespace vendors {
        namespace libraries {

            class LFXE_API LibraryLogitech : public LibraryBase {

            public:
                virtual ~LibraryLogitech();

                virtual bool IsLibraryAvailable() const override;
                virtual bool InitializeLibrary() override;
                virtual bool ReleaseLibrary() override;

                virtual lightfx::Version GetLibraryVerion() const override;

                // Library functions
                bool Logi_Init() const {
                    bool result = LogiLedInit();
                    LOG_DEBUG(L"LogiLedInit(): " + std::to_wstring(result));
                    return result;
                }
                void Logi_Shutdown() const {
                    LogiLedShutdown();
                    LOG_DEBUG(L"LogiLedShutdown()");
                }
                bool Logi_GetSdkVersion(int* majorNum, int* minorNum, int* buildNum) const {
                    bool result = LogiLedGetSdkVersion(majorNum, minorNum, buildNum);
                    LOG_DEBUG(L"LogiLedGetSdkVersion(" + std::to_wstring(*majorNum) + L", " + std::to_wstring(*minorNum) + L", " + std::to_wstring(*buildNum) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SetTargetDevice(int targetDevice) const {
                    bool result = LogiLedSetTargetDevice(targetDevice);
                    LOG_DEBUG(L"LogiLedSetTargetDevice(" + std::to_wstring(targetDevice) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SaveCurrentLighting() const {
                    bool result = LogiLedSaveCurrentLighting();
                    LOG_DEBUG(L"LogiLedSaveCurrentLighting(): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SetLighting(int redPercentage, int greenPercentage, int bluePercentage) const {
                    bool result = LogiLedSetLighting(redPercentage, greenPercentage, bluePercentage);
                    LOG_DEBUG(L"LogiLedSetLighting(" + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_RestoreLighting() const {
                    bool result = LogiLedRestoreLighting();
                    LOG_DEBUG(L"LogiLedRestoreLighting(): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_FlashLighting(int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const {
                    bool result = LogiLedFlashLighting(redPercentage, greenPercentage, bluePercentage, msDuration, msInterval);
                    LOG_DEBUG(L"LogiLedFlashLighting(" + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(msInterval) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_PulseLighting(int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const {
                    bool result = LogiLedPulseLighting(redPercentage, greenPercentage, bluePercentage, msDuration, msInterval);
                    LOG_DEBUG(L"LogiLedPulseLighting(" + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(msInterval) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_StopEffects() const {
                    bool result = LogiLedStopEffects();
                    LOG_DEBUG(L"LogiLedStopEffects(): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SetLightingFromBitmap(unsigned char bitmap[]) const {
                    bool result = LogiLedSetLightingFromBitmap(bitmap);
                    LOG_DEBUG(L"LogiLedSetLightingFromBitmap(...): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SetLightingForKeyWithScanCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const {
                    bool result = LogiLedSetLightingForKeyWithScanCode(keyCode, redPercentage, greenPercentage, bluePercentage);
                    LOG_DEBUG(L"LogiLedSetLightingForKeyWithScanCode(" + std::to_wstring(keyCode) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SetLightingForKeyWithHidCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const {
                    bool result = LogiLedSetLightingForKeyWithHidCode(keyCode, redPercentage, greenPercentage, bluePercentage);
                    LOG_DEBUG(L"LogiLedSetLightingForKeyWithHidCode(" + std::to_wstring(keyCode) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SetLightingForKeyWithQuartzCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const {
                    bool result = LogiLedSetLightingForKeyWithQuartzCode(keyCode, redPercentage, greenPercentage, bluePercentage);
                    LOG_DEBUG(L"LogiLedSetLightingForKeyWithQuartzCode(" + std::to_wstring(keyCode) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SetLightingForKeyWithKeyName(LogiLed::KeyName keyName, int redPercentage, int greenPercentage, int bluePercentage) const {
                    bool result = LogiLedSetLightingForKeyWithKeyName(keyName, redPercentage, greenPercentage, bluePercentage);
                    LOG_DEBUG(L"LogiLedSetLightingForKeyWithKeyName(" + std::to_wstring(keyName) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_SaveLightingForKey(LogiLed::KeyName keyName) const {
                    bool result = LogiLedSaveLightingForKey(keyName);
                    LOG_DEBUG(L"LogiLedSaveLightingForKey(" + std::to_wstring(keyName) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_RestoreLightingForKey(LogiLed::KeyName keyName) const {
                    bool result = LogiLedRestoreLightingForKey(keyName);
                    LOG_DEBUG(L"LogiLedRestoreLightingForKey(" + std::to_wstring(keyName) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_FlashSingleKey(LogiLed::KeyName keyName, int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const{
                    bool result = LogiLedFlashSingleKey(keyName, redPercentage, greenPercentage, bluePercentage, msDuration, msInterval);
                    LOG_DEBUG(L"LogiLedFlashSingleKey(" + std::to_wstring(keyName) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(msInterval) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_PulseSingleKey(LogiLed::KeyName keyName, int startRedPercentage, int startGreenPercentage, int startBluePercentage, int finishRedPercentage, int finishGreenPercentage, int finishBluePercentage, int msDuration, bool isInfinite) const {
                    bool result = LogiLedPulseSingleKey(keyName, startRedPercentage, startGreenPercentage, startBluePercentage, finishRedPercentage, finishGreenPercentage, finishBluePercentage, msDuration, isInfinite);
                    LOG_DEBUG(L"LogiLedPulseSingleKey(" + std::to_wstring(keyName) + L", " + std::to_wstring(startRedPercentage) + L", " + std::to_wstring(startGreenPercentage) + L", " + std::to_wstring(startBluePercentage) + L", " + std::to_wstring(finishRedPercentage) + L", " + std::to_wstring(finishGreenPercentage) + L", " + std::to_wstring(finishBluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(isInfinite) + L"): " + std::to_wstring(result));
                    return result;
                }
                bool Logi_StopEffectsOnKey(LogiLed::KeyName keyName) const {
                    bool result = LogiLedStopEffectsOnKey(keyName);
                    LOG_DEBUG(L"LogiLedStopEffectsOnKey(" + std::to_wstring(keyName) + L"): " + std::to_wstring(result));
                    return result;
                }
            };

        }
    }
}

#pragma once

// Standard includes
#include <string>

// Project includes
#include "LibraryBase.h"

// 3rd party includes
#include "LogitechLEDLib.h"

// API exports
#include "../../Common/ApiExports.h"


namespace lightfx {
    namespace vendors {
        namespace libraries {

            class LFXE_API LibraryLogitech : public LibraryBase {

            public:
                LibraryLogitech() {}
                virtual ~LibraryLogitech();

                virtual bool IsLibraryAvailable() const override;
                virtual bool InitializeLibrary() override;
                virtual bool ReleaseLibrary() override;

                virtual lightfx::Version GetLibraryVerion() const override;

                // Library functions
                bool Logi_Init() const;
                void Logi_Shutdown() const;
                bool Logi_GetSdkVersion(int* majorNum, int* minorNum, int* buildNum) const;
                bool Logi_SetTargetDevice(int targetDevice) const;
                bool Logi_SaveCurrentLighting() const;
                bool Logi_SetLighting(int redPercentage, int greenPercentage, int bluePercentage) const;
                bool Logi_RestoreLighting() const;
                bool Logi_FlashLighting(int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const;
                bool Logi_PulseLighting(int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const;
                bool Logi_StopEffects() const;
                bool Logi_SetLightingFromBitmap(unsigned char bitmap[]) const;
                bool Logi_SetLightingForKeyWithScanCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const;
                bool Logi_SetLightingForKeyWithHidCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const;
                bool Logi_SetLightingForKeyWithQuartzCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const;
                bool Logi_SetLightingForKeyWithKeyName(LogiLed::KeyName keyName, int redPercentage, int greenPercentage, int bluePercentage) const;
                bool Logi_SaveLightingForKey(LogiLed::KeyName keyName) const;
                bool Logi_RestoreLightingForKey(LogiLed::KeyName keyName) const;
                bool Logi_FlashSingleKey(LogiLed::KeyName keyName, int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const;
                bool Logi_PulseSingleKey(LogiLed::KeyName keyName, int startRedPercentage, int startGreenPercentage, int startBluePercentage, int finishRedPercentage, int finishGreenPercentage, int finishBluePercentage, int msDuration, bool isInfinite) const;
                bool Logi_StopEffectsOnKey(LogiLed::KeyName keyName) const;

            };

        }
    }
}

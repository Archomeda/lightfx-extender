#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LibraryLogitech.h"

// Project includes
#include "../../Utils/Log.h"


using namespace std;
using namespace lightfx;

namespace lightfx {
    namespace vendors {
        namespace libraries {

            LFXE_API LibraryLogitech::~LibraryLogitech() {
                this->ReleaseLibrary();
            }

            LFXE_API bool LibraryLogitech::IsLibraryAvailable() const {
                // Sadly, we have to initialize before we can query the SDK version
                // So instead, we hack our way around this by just initializing if we aren't initialized yet
                // If we are already initialized, well... then the library is available isn't it?
                if (!this->isInitialized) {
                    bool result = this->Logi_Init();
                    this->Logi_Shutdown();
                    return result;
                }
                return true;
            }

            LFXE_API bool LibraryLogitech::InitializeLibrary() {
                if (!this->isInitialized) {
                    LOG_INFO(L"Initializing Logitech library...");
                    this->isInitialized = this->Logi_Init();
                    if (this->isInitialized) {
                        LOG_INFO(L"Logitech library initialized");
                    } else {
                        LOG_WARNING(L"Could not initialize Logitech library");
                    }
                }
                return this->isInitialized;
            }

            LFXE_API bool LibraryLogitech::ReleaseLibrary() {
                if (this->isInitialized) {
                    LOG_INFO(L"Releasing Logitech library...");
                    this->Logi_SetTargetDevice(LOGI_DEVICETYPE_ALL);
                    this->Logi_RestoreLighting();
                    this->Logi_Shutdown();
                    this->isInitialized = false;
                    LOG_INFO(L"Logitech library released");
                }
                return true;
            }

            LFXE_API Version LibraryLogitech::GetLibraryVerion() const {
                // Sadly, we have to initialize before we can query the SDK version
                // So if we aren't initialized, let's do that first and shut down afterwards
                if (!this->isInitialized) {
                    bool result = this->Logi_Init();
                    if (!result) {
                        return{};
                    }
                }
                int major = 0, minor = 0, build = 0;
                Version version;
                if (this->Logi_GetSdkVersion(&major, &minor, &build)) {
                    version = Version(major, minor, build, 0);
                }
                if (!this->isInitialized) {
                    this->Logi_Shutdown();
                }
                return version;
            }

            LFXE_API bool LibraryLogitech::Logi_Init() const {
                bool result = LogiLedInit();
                LOG_DEBUG(L"LogiLedInit(): " + std::to_wstring(result));
                return result;
            }

            LFXE_API void LibraryLogitech::Logi_Shutdown() const {
                LogiLedShutdown();
                LOG_DEBUG(L"LogiLedShutdown()");
            }

            LFXE_API bool LibraryLogitech::Logi_GetSdkVersion(int* majorNum, int* minorNum, int* buildNum) const {
                bool result = LogiLedGetSdkVersion(majorNum, minorNum, buildNum);
                LOG_DEBUG(L"LogiLedGetSdkVersion(" + std::to_wstring(*majorNum) + L", " + std::to_wstring(*minorNum) + L", " + std::to_wstring(*buildNum) + L"): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_SetTargetDevice(int targetDevice) const {
                bool result = LogiLedSetTargetDevice(targetDevice);
                LOG_DEBUG(L"LogiLedSetTargetDevice(" + std::to_wstring(targetDevice) + L"): " + std::to_wstring(result));
                return result;
            }
         
            LFXE_API bool LibraryLogitech::Logi_SaveCurrentLighting() const {
                bool result = LogiLedSaveCurrentLighting();
                LOG_DEBUG(L"LogiLedSaveCurrentLighting(): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_SetLighting(int redPercentage, int greenPercentage, int bluePercentage) const {
                bool result = LogiLedSetLighting(redPercentage, greenPercentage, bluePercentage);
                LOG_DEBUG(L"LogiLedSetLighting(" + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_RestoreLighting() const {
                bool result = LogiLedRestoreLighting();
                LOG_DEBUG(L"LogiLedRestoreLighting(): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_FlashLighting(int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const {
                bool result = LogiLedFlashLighting(redPercentage, greenPercentage, bluePercentage, msDuration, msInterval);
                LOG_DEBUG(L"LogiLedFlashLighting(" + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(msInterval) + L"): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_PulseLighting(int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const {
                bool result = LogiLedPulseLighting(redPercentage, greenPercentage, bluePercentage, msDuration, msInterval);
                LOG_DEBUG(L"LogiLedPulseLighting(" + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(msInterval) + L"): " + std::to_wstring(result));
                return result;
            }
           
            LFXE_API bool LibraryLogitech::Logi_StopEffects() const {
                bool result = LogiLedStopEffects();
                LOG_DEBUG(L"LogiLedStopEffects(): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_SetLightingFromBitmap(unsigned char bitmap[]) const {
                bool result = LogiLedSetLightingFromBitmap(bitmap);
                LOG_DEBUG(L"LogiLedSetLightingFromBitmap(...): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_SetLightingForKeyWithScanCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const {
                bool result = LogiLedSetLightingForKeyWithScanCode(keyCode, redPercentage, greenPercentage, bluePercentage);
                LOG_DEBUG(L"LogiLedSetLightingForKeyWithScanCode(" + std::to_wstring(keyCode) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                return result;
            }
         
            LFXE_API bool LibraryLogitech::Logi_SetLightingForKeyWithHidCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const {
                bool result = LogiLedSetLightingForKeyWithHidCode(keyCode, redPercentage, greenPercentage, bluePercentage);
                LOG_DEBUG(L"LogiLedSetLightingForKeyWithHidCode(" + std::to_wstring(keyCode) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                return result;
            }
           
            LFXE_API bool LibraryLogitech::Logi_SetLightingForKeyWithQuartzCode(int keyCode, int redPercentage, int greenPercentage, int bluePercentage) const {
                bool result = LogiLedSetLightingForKeyWithQuartzCode(keyCode, redPercentage, greenPercentage, bluePercentage);
                LOG_DEBUG(L"LogiLedSetLightingForKeyWithQuartzCode(" + std::to_wstring(keyCode) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_SetLightingForKeyWithKeyName(LogiLed::KeyName keyName, int redPercentage, int greenPercentage, int bluePercentage) const {
                bool result = LogiLedSetLightingForKeyWithKeyName(keyName, redPercentage, greenPercentage, bluePercentage);
                LOG_DEBUG(L"LogiLedSetLightingForKeyWithKeyName(" + std::to_wstring(keyName) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L"): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_SaveLightingForKey(LogiLed::KeyName keyName) const {
                bool result = LogiLedSaveLightingForKey(keyName);
                LOG_DEBUG(L"LogiLedSaveLightingForKey(" + std::to_wstring(keyName) + L"): " + std::to_wstring(result));
                return result;
            }
         
            LFXE_API bool LibraryLogitech::Logi_RestoreLightingForKey(LogiLed::KeyName keyName) const {
                bool result = LogiLedRestoreLightingForKey(keyName);
                LOG_DEBUG(L"LogiLedRestoreLightingForKey(" + std::to_wstring(keyName) + L"): " + std::to_wstring(result));
                return result;
            }
           
            LFXE_API bool LibraryLogitech::Logi_FlashSingleKey(LogiLed::KeyName keyName, int redPercentage, int greenPercentage, int bluePercentage, int msDuration, int msInterval) const{
                bool result = LogiLedFlashSingleKey(keyName, redPercentage, greenPercentage, bluePercentage, msDuration, msInterval);
                LOG_DEBUG(L"LogiLedFlashSingleKey(" + std::to_wstring(keyName) + L", " + std::to_wstring(redPercentage) + L", " + std::to_wstring(greenPercentage) + L", " + std::to_wstring(bluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(msInterval) + L"): " + std::to_wstring(result));
                return result;
            }
           
            LFXE_API bool LibraryLogitech::Logi_PulseSingleKey(LogiLed::KeyName keyName, int startRedPercentage, int startGreenPercentage, int startBluePercentage, int finishRedPercentage, int finishGreenPercentage, int finishBluePercentage, int msDuration, bool isInfinite) const {
                bool result = LogiLedPulseSingleKey(keyName, startRedPercentage, startGreenPercentage, startBluePercentage, finishRedPercentage, finishGreenPercentage, finishBluePercentage, msDuration, isInfinite);
                LOG_DEBUG(L"LogiLedPulseSingleKey(" + std::to_wstring(keyName) + L", " + std::to_wstring(startRedPercentage) + L", " + std::to_wstring(startGreenPercentage) + L", " + std::to_wstring(startBluePercentage) + L", " + std::to_wstring(finishRedPercentage) + L", " + std::to_wstring(finishGreenPercentage) + L", " + std::to_wstring(finishBluePercentage) + L", " + std::to_wstring(msDuration) + L", " + std::to_wstring(isInfinite) + L"): " + std::to_wstring(result));
                return result;
            }
          
            LFXE_API bool LibraryLogitech::Logi_StopEffectsOnKey(LogiLed::KeyName keyName) const {
                bool result = LogiLedStopEffectsOnKey(keyName);
                LOG_DEBUG(L"LogiLedStopEffectsOnKey(" + std::to_wstring(keyName) + L"): " + std::to_wstring(result));
                return result;
            }

        }
    }
}

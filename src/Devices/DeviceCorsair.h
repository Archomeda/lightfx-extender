#pragma once

// Windows includes
#include "Device.h"

// API exports
#include "../Common/ApiExports.h"

#include "CUESDK.h"


// The CUESDK doesn't allow explicit linking, so we have to do it ourselves.
// The reason why we want explicit linking, is that we do not want to depend on CUESDK_2013.dll
// or its x64 equivalent if it's not available (it may cause LFXE to not load when the DLL is missing).
//! Keep in mind that we have to manually update this every time the SDK has been updated.

typedef bool(*CORSAIRSETLEDSCOLORS)(int, CorsairLedColor*);
typedef bool(*CORSAIRSETLEDSCOLORSASYNC)(int, CorsairLedColor*, void(*CallbackType)(void*, bool, CorsairError), void*);
typedef int(*CORSAIRGETDEVICECOUNT)();
typedef CorsairDeviceInfo*(*CORSAIRGETDEVICEINFO)(int);
typedef CorsairLedPositions*(*CORSAIRGETLEDPOSITIONS)();
typedef CorsairLedId(*CORSAIRGETLEDIDFORKEYNAME)(char);
typedef bool(*CORSAIRREQUESTCONTROL)(CorsairAccessMode);
typedef CorsairProtocolDetails(*CORSAIRPERFORMPROTOCOLHANDSHAKE)();
typedef CorsairError(*CORSAIRGETLASTERROR)();

#ifdef _WIN64
#define CORSAIR_DLL_NAME "CUESDK.x64_2013.dll"
#else
#define CORSAIR_DLL_NAME "CUESDK_2013.dll"
#endif

#define CORSAIR_DLL_SETLEDSCOLORS "CorsairSetLedsColors"
#define CORSAIR_DLL_SETLEDSCOLORSASYNC "CorsairSetLedsColorsAsync"
#define CORSAIR_DLL_GETDEVICECOUNT "CorsairGetDeviceCount"
#define CORSAIR_DLL_GETDEVICEINFO "CorsairGetDeviceInfo"
#define CORSAIR_DLL_GETLEDPOSITIONS "CorsairGetLedPositions"
#define CORSAIR_DLL_GETLEDIDFORKEYNAME "CorsairGetLedIdForKeyName"
#define CORSAIR_DLL_REQUESTCONTROL "CorsairRequestControl"
#define CORSAIR_DLL_PERFORMPROTOCOLHANDSHAKE "CorsairPerformProtocolHandshake"
#define CORSAIR_DLL_GETLASTERROR "CorsairGetLastError"

extern CORSAIRSETLEDSCOLORS Corsair_SetLedsColors;
extern CORSAIRSETLEDSCOLORSASYNC Corsair_SetLedsColorsAsync;
extern CORSAIRGETDEVICECOUNT Corsair_GetDeviceCount;
extern CORSAIRGETDEVICEINFO Corsair_GetDeviceInfo;
extern CORSAIRGETLEDPOSITIONS Corsair_GetLedPositions;
extern CORSAIRGETLEDIDFORKEYNAME Corsair_GetLedIdForKeyName;
extern CORSAIRREQUESTCONTROL Corsair_RequestControl;
extern CORSAIRPERFORMPROTOCOLHANDSHAKE Corsair_PerformProtocolHandshake;
extern CORSAIRGETLASTERROR Corsair_GetLastError;


namespace lightfx {
    namespace devices {

        class LFXE_API DeviceCorsair : public Device {

        public:
            void SetRange(const int outMin, const int outMax, const int inMin, const int inMax);

            virtual bool Initialize() override;
            virtual bool Release() override;

            virtual bool Enable() override;

            virtual const std::wstring GetDeviceName() override { return L"Corsair"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceKeyboard; }

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

        private:
            int rangeOutMin = 0;
            int rangeOutMax = 255;
            int rangeInMin = 0;
            int rangeInMax = 255;
            CorsairLedPositions* ledPositions;

            const char* toString(CorsairError error);
        };

    }
}

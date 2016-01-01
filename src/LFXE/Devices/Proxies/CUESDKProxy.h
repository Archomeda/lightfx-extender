#pragma once

// Standard includes
#include <string>

// Windows includes
#include "../../Common/Windows.h"
#include <Windows.h>

// 3rd party includes
#include "CUESDK.h"

// Project includes
#include "DeviceLibraryProxy.h"

// API exports
#include "../../Common/ApiExports.h"


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


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace devices {
        namespace proxies {

            class LFXE_API CUESDKProxy : DeviceLibraryProxy {

            public:
                CUESDKProxy() {}

                virtual bool Load() override;
                virtual bool Unload() override;

                using DeviceLibraryProxy::IsLoaded;

                std::wstring CorsairErrorToString(const CorsairError error);
                
                // CUESDK function declarations
                CORSAIRSETLEDSCOLORS CorsairSetLedsColors;
                CORSAIRSETLEDSCOLORSASYNC CorsairSetLedsColorsAsync;
                CORSAIRGETDEVICECOUNT CorsairGetDeviceCount;
                CORSAIRGETDEVICEINFO CorsairGetDeviceInfo;
                CORSAIRGETLEDPOSITIONS CorsairGetLedPositions;
                CORSAIRGETLEDIDFORKEYNAME CorsairGetLedIdForKeyName;
                CORSAIRREQUESTCONTROL CorsairRequestControl;
                CORSAIRPERFORMPROTOCOLHANDSHAKE CorsairPerformProtocolHandshake;
                CORSAIRGETLASTERROR CorsairGetLastError;

            protected:
                bool ReleaseLibrary();

            private:
                HINSTANCE hInstance = NULL;

            };

        }
    }
}

#pragma warning(pop)

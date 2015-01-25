#pragma once

// Standard includes
#include <memory>
#include <string>

// Windows includes
#include <Windows.h>

// 3rd party includes
#include "LFXDecl.h"


#define MMF_MAXDEVICES 8
#define MMF_MAXLIGHTSPERDEVICE 16
#define MMF_MAXLENGTHNAME 32

namespace lightfx {

    struct LFXLight {
        char name[MMF_MAXLENGTHNAME];
        LFX_POSITION position;
        uint32_t actionType;
        LFX_COLOR primaryColor;
        LFX_COLOR secondaryColor;
    };

    struct LFXDevice {
        char name[MMF_MAXLENGTHNAME];
        char type;
        uint32_t numLights;
        LFXLight lights[MMF_MAXLIGHTSPERDEVICE];
    };

    struct LinkedMem {
        uint32_t version;
        uint32_t tick;

        uint32_t isInitialized;
        int32_t timing;

        uint32_t numDevices;
        LFXDevice devices[MMF_MAXDEVICES];
    };

    class MemoryMappedFileApi {

    public:
        std::unique_ptr<LinkedMem> Handle = nullptr;

        MemoryMappedFileApi(const std::string& name);
        ~MemoryMappedFileApi();

        bool Opened();
        bool Open();
        void Close();
                
    private:
        std::string name = "";
        HANDLE hMapObject = NULL;

        bool isOpen = false;

    };

}

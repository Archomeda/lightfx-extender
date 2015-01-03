#pragma once
#include <memory>
#include <string>
#include <Windows.h>
#include "LFXDecl.h"

namespace lightfx {

    struct LFXLight {
        LFX_POSITION position;
        uint32_t actionType;
        LFX_COLOR primaryColor;
        LFX_COLOR secondaryColor;
    };

    struct LinkedMem {
        uint32_t version;
        uint32_t tick;

        uint32_t isInitialized;
        int32_t timing;

        LFXLight light;
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

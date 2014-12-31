#include <windows.h>
#include "LinkedMemAPI.h"


namespace lightfx {
    namespace api {

        HANDLE hMapObject = NULL;
        LinkedMem* linkedMemHandle = NULL;

        void init() {
            hMapObject = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LFXLight), L"AlienFXFrameworkLink");
            if (hMapObject == NULL) {
                return;
            }

            linkedMemHandle = (LinkedMem*)MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LFXLight));
            if (linkedMemHandle == NULL) {
                dispose();
            }

            linkedMemHandle->version = 0;
        }

        void dispose() {
            if (linkedMemHandle != NULL) {
                UnmapViewOfFile(linkedMemHandle);
                linkedMemHandle = NULL;
            }
            if (hMapObject != NULL) {
                CloseHandle(hMapObject);
                hMapObject = NULL;
            }
        }

    }
}

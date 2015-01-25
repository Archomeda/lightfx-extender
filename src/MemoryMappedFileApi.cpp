#include "MemoryMappedFileApi.h"


using namespace std;

#define APIVER 0

namespace lightfx {

    MemoryMappedFileApi::MemoryMappedFileApi(const string& name) {
        this->name = name;
    }

    MemoryMappedFileApi::~MemoryMappedFileApi() {
        this->Close();
    }

    bool MemoryMappedFileApi::Opened() {
        return this->isOpen;
    }

    bool MemoryMappedFileApi::Open() {
        this->hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LinkedMem), this->name.c_str());
        if (this->hMapObject == NULL) {
            return false;
        }

        this->Handle = unique_ptr<LinkedMem>((LinkedMem*)MapViewOfFile(this->hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem)));
        if (this->Handle == nullptr) {
            this->Close();
            return false;
        }

        this->Handle->version = APIVER;
        this->isOpen = true;
        return true;
    }

    void MemoryMappedFileApi::Close() {
        if (this->Handle != nullptr) {
            UnmapViewOfFile(this->Handle.get());
            this->Handle.release();
        }

        if (this->hMapObject != NULL) {
            CloseHandle(this->hMapObject);
            hMapObject = NULL;
        }

        this->isOpen = false;
    }

}

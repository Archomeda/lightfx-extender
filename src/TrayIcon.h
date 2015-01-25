#pragma once

// Standard includes
#include <vector>
#include <memory>

// Windows includes
#include "Common/Windows.h"
#include <Windows.h>
#include <shellapi.h>

// Project includes
#include "Devices/DeviceBase.h"


namespace lightfx {

    class TrayIcon {

    public:
        bool AddTrayIcon();
        bool RemoveTrayIcon();

        void AttachDevice(std::shared_ptr<devices::DeviceBase> device);
        void DetachDevice(std::shared_ptr<devices::DeviceBase> device);

    private:
        HINSTANCE hModuleInstance = {};
        WNDCLASSW trayIconWindowClass = {};
        HWND hTrayIconWindow = {};
        NOTIFYICONDATAW trayIconData = {};

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void TrayIconCallback(WPARAM wParam, LPARAM lParam);

        std::vector<std::shared_ptr<devices::DeviceBase>> attachedDevices;

    };

}
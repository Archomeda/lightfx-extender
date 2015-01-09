#pragma once

#define NTDDI_VERSION NTDDI_WIN7

#include <memory>
#include "Config.h"
#include "DeviceLogitech.h"
#include "DeviceLightpack.h"
#include "DeviceMmf.h"
#include "LightFX.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace lightfx {

    class DeviceManager {

    public:
        ~DeviceManager();
        LightFX* GetLightFXManager();

        bool Initialize();
        bool Dispose();

        bool EnableLogitechDevice();
        bool DisableLogitechDevice();
        bool EnableLightpackDevice();
        bool DisableLightpackDevice();
        bool EnableMmfDevice();
        bool DisableMmfDevice();

    private:
        Config config;
        LightFX lightFXManager;

        bool isInitialized = false;

        std::shared_ptr<devices::DeviceLogitech> deviceLogitech;
        std::shared_ptr<devices::DeviceLightpack> deviceLightpack;
        std::vector<std::shared_ptr<devices::DeviceMmf>> deviceMemoryMappedFile;
        bool deviceMemoryMappedFileEnabled = false;

#pragma region Tray Icon
        HINSTANCE hModuleInstance = {};
        WNDCLASS trayIconWindowClass = {};
        HWND hTrayIconWindow = {};
        NOTIFYICONDATA trayIconData = {};

        bool AddTrayIcon();
        bool RemoveTrayIcon();
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void TrayIconCallback(WPARAM wParam, LPARAM lParam);
#pragma endregion

    };

}

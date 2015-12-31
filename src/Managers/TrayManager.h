#pragma once

// Standard includes
#include <string>
#include <thread>
#include <vector>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>
#include <shellapi.h>

// Project includes
#include "Manager.h"
#include "../Devices/Device.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    class LightFXExtender;

    namespace managers {

        class LFXE_API TrayManager : public Manager < TrayManager > {

        public:
            TrayManager() {}

            bool IsAdded();
            void AddTrayIcon();
            void RemoveTrayIcon();

            bool HasUpdateNotification();
            void SetUpdateNotification(const std::wstring& versionString, const std::wstring& downloadUrl);

        protected:
            std::wstring updateVersionString;
            std::wstring updateVersionUrl;

            HINSTANCE hModuleInstance = {};
            WNDCLASSW trayIconWindowClass = {};
            HWND hTrayIconWindow = {};
            NOTIFYICONDATAW trayIconData = {};

            bool isTrayIconAdded = false;
            bool isTrayIconRemoving = false;
            std::thread trayIconThread;

            void AddTrayIconThreaded();

            static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
            void TrayIconCallback(WPARAM wParam, LPARAM lParam);

        };

    }
}

#pragma warning(pop)

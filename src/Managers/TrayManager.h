#pragma once

// Standard includes
#include <string>
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

            void AttachDevice(const std::shared_ptr<devices::Device>& device);
            std::shared_ptr<devices::Device> GetDevice(const std::wstring& deviceName);
            void RemoveDevice(const std::wstring& deviceName);

            bool HasUpdateNotification();
            void SetUpdateNotification(const std::wstring& versionString, const std::wstring& downloadUrl);

        protected:
            std::vector<std::weak_ptr<devices::Device>> devices;
            std::wstring updateVersionString;
            std::wstring updateVersionUrl;

            HINSTANCE hModuleInstance = {};
            WNDCLASSW trayIconWindowClass = {};
            HWND hTrayIconWindow = {};
            NOTIFYICONDATAW trayIconData = {};

            bool isTrayIconAdded = false;

            static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
            void TrayIconCallback(WPARAM wParam, LPARAM lParam);

        };

    }
}

#pragma warning(pop)

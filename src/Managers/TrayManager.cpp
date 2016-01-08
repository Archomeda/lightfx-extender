#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "TrayManager.h"

// Standard includes
#include <algorithm>

// Windows includes
#include <strsafe.h>

// Project includes
#include "../LightFXExtender.h"
#include "ConfigManager.h"
#include "DeviceManager.h"
#include "UpdateManager.h"
#include "../Config/MainConfigFile.h"
#include "../Utils/FileIO.h"
#include "../Utils/Log.h"
#include "../Utils/Windows.h"
#include "../resource.h"


#define WINDOW_CLASSNAME L"TrayIcon"
#define TRAYID 1
#define WM_TRAYICON WM_USER + 1
#define WM_TRAYICON_CLOSE WM_USER + 100

#define MENU_CONFFOLDER_NAME L"Open &configuration folder"
#define MENU_UPDATE_NAME L"&Download new version"

#define TRAY_BALLOON_TITLE L"LightFX Extender"
#define TRAY_BALLOON_UPDATE_TEXT L" is available.\nClick here to download it."


using namespace std;
using namespace lightfx::config;
using namespace lightfx::devices;
using namespace lightfx::utils;

namespace lightfx {
    namespace managers {

        LFXE_API bool TrayManager::IsAdded() {
            return nullptr;
        }

        LFXE_API void TrayManager::AddTrayIcon() {
            if (this->isTrayIconAdded) {
                return;
            }

            this->trayIconThread = thread(&TrayManager::AddTrayIconThreaded, this);
        }

        LFXE_API void TrayManager::RemoveTrayIcon() {
            if (!this->isTrayIconAdded) {
                return;
            }

            SendMessage(this->hTrayIconWindow, WM_TRAYICON_CLOSE, 0, 0);
            this->trayIconThread.join();

            if (!this->isTrayIconAdded) {
                LOG_DEBUG(L"Tray icon removed");
            } else {
                LOG_DEBUG(L"Failed to remove tray icon");
            }
        }


        LFXE_API void TrayManager::ResetNotification() {
            this->trayIconData.dwInfoFlags = NIIF_INFO | NIIF_LARGE_ICON | 0x80;
            StringCchCopyW(this->trayIconData.szInfo, ARRAYSIZE(this->trayIconData.szInfo), L"");
            Shell_NotifyIconW(NIM_MODIFY, &this->trayIconData);
        }

        LFXE_API bool TrayManager::HasUpdateNotification() {
            return !this->updateVersionString.empty() && !this->updateVersionUrl.empty();
        }

        LFXE_API void TrayManager::SetUpdateNotification(const wstring& versionString, const wstring& downloadUrl) {
            this->updateVersionString = versionString;
            this->updateVersionUrl = downloadUrl;

            this->trayIconData.dwInfoFlags = NIIF_USER | NIIF_LARGE_ICON | 0x80;
            StringCchCopyW(this->trayIconData.szInfoTitle, ARRAYSIZE(this->trayIconData.szInfoTitle), TRAY_BALLOON_TITLE);
            StringCchCopyW(this->trayIconData.szInfo, ARRAYSIZE(this->trayIconData.szInfo), (L"Version " + versionString + TRAY_BALLOON_UPDATE_TEXT).c_str());
            Shell_NotifyIconW(NIM_MODIFY, &this->trayIconData);
        }


        unsigned int TrayManager::GetCurrentIconIndex() {
            bool active = false;
            auto deviceManager = this->GetLightFXExtender()->GetDeviceManager();
            for (size_t i = 0; i < deviceManager->GetChildrenCount(); ++i) {
                auto device = deviceManager->GetChildByIndex(i);
                if (device->IsInitialized() && device->IsEnabled()) {
                    active = true;
                    break;
                }
            }
            return active ? IDI_TRAYICON1 : IDI_TRAYICON2;
        }

        void TrayManager::UpdateIcon(const bool callShellModify) {
            if (!this->GetLightFXExtender()->GetConfigManager()->GetMainConfig()->TrayIconUseGameIcon) {
                this->trayIconData.hIcon = (HICON)LoadImageW(this->hModuleInstance, MAKEINTRESOURCEW(this->GetCurrentIconIndex()), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
                if (callShellModify) {
                    this->ResetNotification();
                    Shell_NotifyIconW(NIM_MODIFY, &this->trayIconData);
                }
            }
        }

        void TrayManager::AddTrayIconThreaded() {
            this->hModuleInstance = GetCurrentModule();
            this->trayIconWindowClass.lpfnWndProc = &TrayManager::WndProc;
            this->trayIconWindowClass.hInstance = this->hModuleInstance;
            this->trayIconWindowClass.lpszClassName = WINDOW_CLASSNAME;
            this->trayIconWindowClass.cbWndExtra = sizeof(TrayManager*);
            RegisterClassW(&this->trayIconWindowClass);
            this->hTrayIconWindow = CreateWindowExW(0, WINDOW_CLASSNAME, L"LightFX Extender Tray Icon", 0, 0, 0, 0, 0, NULL, NULL, this->hModuleInstance, static_cast<LPVOID>(this));

            wstring fname, ext;
            wstring filename = GetProcessName(nullptr, nullptr, &fname, &ext);

            wstring currentVersion = this->GetLightFXExtender()->GetUpdateManager()->GetCurrentVersion().ToString();
            this->trayIconData = {};
            this->trayIconData.cbSize = sizeof(NOTIFYICONDATAW);
            this->trayIconData.hWnd = this->hTrayIconWindow;
            this->trayIconData.uID = TRAYID;
            this->trayIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
            StringCchCopyW(this->trayIconData.szTip, ARRAYSIZE(this->trayIconData.szTip), (L"LightFX for " + fname + ext + L" (v" + currentVersion + L")").c_str());
            this->trayIconData.uVersion = NOTIFYICON_VERSION_4;
            this->trayIconData.uCallbackMessage = WM_TRAYICON;

            // Check whether the executable icon or the default icon should be used
            if (this->GetLightFXExtender()->GetConfigManager()->GetMainConfig()->TrayIconUseGameIcon) {
                this->trayIconData.hIcon = ExtractIconW(GetModuleHandle(NULL), filename.c_str(), 0);
            }
            if (this->trayIconData.hIcon == NULL) {
                // Fall back to our default icon if the executable icon cannot be found or is not configured to be used
                this->UpdateIcon(false);
                this->trayIconData.hBalloonIcon = (HICON)LoadImageW(this->hModuleInstance, MAKEINTRESOURCEW(IDI_TRAYICON1), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
            }

            this->isTrayIconAdded = Shell_NotifyIconW(NIM_ADD, &this->trayIconData) == TRUE;
            if (this->isTrayIconAdded) {
                LOG_DEBUG(L"Tray icon added");
            } else {
                LOG_DEBUG(L"Failed to add tray icon");
            }


            // Process the message loop until we remove the icon
            MSG msg;
            BOOL hasMsg;
            while ((hasMsg = GetMessage(&msg, NULL, 0, 0)) != 0) {
                if (hasMsg == -1) {
                    // Error
                    LOG_ERROR(L"Error in processing tray icon message loop");
                    break;
                } else {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }

        LRESULT CALLBACK TrayManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            if (msg == WM_NCCREATE) {
                // Set extra window space to point to the this object
                CREATESTRUCT* pCs = reinterpret_cast<CREATESTRUCT*>(lParam);
                LPVOID pThis = pCs->lpCreateParams;
                SetWindowLongPtrW(hWnd, 0, reinterpret_cast<LONG_PTR>(pThis));
            }

            // Retrieve the pointer to this from the extra window space
            TrayManager* that = reinterpret_cast<TrayManager*>(GetWindowLongPtrW(hWnd, 0));

            switch (msg) {
            case WM_TRAYICON:
                that->TrayIconCallback(wParam, lParam);
                break;
            case WM_TRAYICON_CLOSE:
                bool result = Shell_NotifyIconW(NIM_DELETE, &that->trayIconData) == TRUE;
                DestroyWindow(that->hTrayIconWindow);
                that->hTrayIconWindow = NULL;
                UnregisterClassW(WINDOW_CLASSNAME, that->hModuleInstance);
                that->hModuleInstance = NULL;
                that->isTrayIconAdded = !result;
                PostQuitMessage(0);
                break;
            }
            return DefWindowProcW(hWnd, msg, wParam, lParam);
        };

        void TrayManager::TrayIconCallback(WPARAM wParam, LPARAM lParam) {
            if (wParam == TRAYID) {
                if (lParam == WM_RBUTTONUP) {
                    HMENU hMenu = CreatePopupMenu();

                    size_t index;
                    auto deviceManager = this->GetLightFXExtender()->GetDeviceManager();
                    for (index = 1; index <= deviceManager->GetChildrenCount(); ++index) {
                        auto device = deviceManager->GetChildByIndex(index - 1);
                        if (device->IsInitialized()) {
                            InsertMenuW(hMenu, static_cast<UINT>(index), device->IsEnabled() ? MF_CHECKED : MF_UNCHECKED, static_cast<UINT>(index), device->GetDeviceName().c_str());
                        }
                    }
                    InsertMenuW(hMenu, static_cast<UINT>(index), MF_SEPARATOR, 0, NULL);
                    ++index;
                    UINT updateUrlIndex = 0;
                    if (this->updateVersionString != L"") {
                        updateUrlIndex = static_cast<UINT>(index);
                        InsertMenuW(hMenu, updateUrlIndex, 0, updateUrlIndex, wstring(wstring(MENU_UPDATE_NAME) + L" (v" + this->updateVersionString + L")...").c_str());
                        ++index;
                    }
                    const UINT confDirIndex = static_cast<UINT>(index);
                    InsertMenuW(hMenu, confDirIndex, 0, confDirIndex, MENU_CONFFOLDER_NAME);

                    POINT cursor;
                    GetCursorPos(&cursor);
                    SetForegroundWindow(this->hTrayIconWindow);
                    UINT result = TrackPopupMenu(hMenu, TPM_RETURNCMD, cursor.x, cursor.y, 0, this->hTrayIconWindow, NULL);
                    DestroyMenu(hMenu);

                    if (result == confDirIndex) {
                        ShellExecuteW(NULL, L"explore", GetDataStorageFolder().c_str(), NULL, NULL, SW_SHOWNORMAL);
                    } else if (updateUrlIndex > 0 && result == updateUrlIndex) {
                        ShellExecuteW(NULL, L"open", this->updateVersionUrl.c_str(), NULL, NULL, SW_SHOWDEFAULT);
                    } else if (result > 0 && result <= deviceManager->GetChildrenCount()) {
                        auto device = deviceManager->GetChildByIndex(result - 1);
                        if (device->IsEnabled()) {
                            if (!device->Disable()) {
                                MessageBoxW(NULL, wstring(L"Failed to disable " + device->GetDeviceName() + L".\r\nCheck the log to see if there are more details.").c_str(), L"LightFX Extender", MB_OK | MB_ICONERROR);
                            }
                        } else {
                            if (!device->Enable()) {
                                MessageBoxW(NULL, wstring(L"Failed to enable " + device->GetDeviceName() + L".\r\nCheck the log to see if there are more details.").c_str(), L"LightFX Extender", MB_OK | MB_ICONERROR);
                            }
                        }
                    }

                    this->UpdateIcon();
                } else if (lParam == NIN_BALLOONUSERCLICK) {
                    if (this->updateVersionUrl != L"") {
                        ShellExecuteW(NULL, L"open", this->updateVersionUrl.c_str(), NULL, NULL, SW_SHOWDEFAULT);
                    }
                }
            }
        }

    }
}

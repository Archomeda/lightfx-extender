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
#include "UpdateManager.h"
#include "VendorManager.h"
#include "../Config/MainConfigFile.h"
#include "../Vendors/VendorDevice.h"
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
using namespace lightfx::vendors;
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


        LFXE_API bool TrayManager::HasUpdateNotification() {
            return !this->updateVersionString.empty() && !this->updateVersionUrl.empty();
        }

        LFXE_API void TrayManager::SetUpdateNotification(const wstring& versionString, const wstring& downloadUrl) {
            this->updateVersionString = versionString;
            this->updateVersionUrl = downloadUrl;

            this->trayIconData.dwInfoFlags = NIIF_INFO | 0x80;
            StringCchCopyW(this->trayIconData.szInfoTitle, ARRAYSIZE(this->trayIconData.szInfoTitle), TRAY_BALLOON_TITLE);
            StringCchCopyW(this->trayIconData.szInfo, ARRAYSIZE(this->trayIconData.szInfo), (L"Version " + versionString + TRAY_BALLOON_UPDATE_TEXT).c_str());
            Shell_NotifyIconW(NIM_MODIFY, &this->trayIconData);
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

            // Not sure if taking the icon from an EXE file is desired by some companies,
            // as it can cause confusion to users who might think it's officially supported by those companies.
            // Therefore, it is an advanced option which is disabled by default
            if (this->GetLightFXExtender()->GetConfigManager()->GetMainConfig()->TrayIconUseGameIcon) {
                this->trayIconData.hIcon = ExtractIconW(GetModuleHandle(NULL), filename.c_str(), 0);
            }
            if (this->trayIconData.hIcon == NULL) {
                // Fall back to our default (somewhat crappy) icon if the executable icon cannot be found or is not configured to be used
                this->trayIconData.hIcon = (HICON)LoadImageW(this->hModuleInstance, MAKEINTRESOURCEW(IDI_TRAYICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
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
                    vector<HMENU> hSubMenus = {};
                    map<UINT, shared_ptr<VendorDevice>> deviceMap = {};
                    auto vendorManager = this->GetLightFXExtender()->GetVendorManager();
                    auto vendors = vendorManager->GetVendors();

                    // Add devices
                    size_t index;
                    for (index = 1; index <= vendors.size(); ++index) {
                        auto vendor = vendors[index - 1];
                        if (vendor->IsLibraryAvailable()) {
                            HMENU hSubMenu = CreatePopupMenu();
                            hSubMenus.push_back(hSubMenu);
                            size_t subIndex = 1;
                            for (auto pair : vendor->GetDevices()) {
                                auto device = pair.second;
                                UINT id = static_cast<UINT>(index * 100 + subIndex);
                                AppendMenuW(hSubMenu,
                                    device->IsEnabled() ? MF_CHECKED : MF_UNCHECKED,
                                    id,
                                    device->GetDeviceName().c_str()
                                );
                                deviceMap.emplace(id, device);
                                ++subIndex;
                            }
                            InsertMenuW(hMenu,
                                static_cast<UINT>(index),
                                MF_POPUP,
                                (UINT_PTR)hSubMenu,
                                vendor->GetVendorName().c_str()
                            );
                        }
                    }

                    // Add other stuff
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
                    for (HMENU hSubMenu : hSubMenus) {
                        DestroyMenu(hSubMenu);
                    }

                    if (result == confDirIndex) {
                        ShellExecuteW(NULL, L"explore", GetDataStorageFolder().c_str(), NULL, NULL, SW_SHOWNORMAL);
                    } else if (updateUrlIndex > 0 && result == updateUrlIndex) {
                        ShellExecuteW(NULL, L"open", this->updateVersionUrl.c_str(), NULL, NULL, SW_SHOWDEFAULT);
                    } else if (result > 100) {
                        if (deviceMap.find(result) != deviceMap.end()) {
                            auto device = deviceMap[result];
                            wstring deviceName = device->GetVendor()->GetVendorName() + L" - " + device->GetDeviceName();
                            if (device->IsEnabled()) {
                                LOG_INFO(L"Disabling hardware " + deviceName + L"...");
                                if (device->Disable()) {
                                    LOG_INFO(L"Disabled hardware " + deviceName);
                                } else {
                                    LOG_WARNING(L"Could not disable hardware " + deviceName);
                                    MessageBoxW(NULL, wstring(L"Failed to disable " + deviceName + L".\r\nCheck the log to see if there are more details.").c_str(), L"LightFX Extender", MB_OK | MB_ICONERROR);
                                }
                            } else {
                                LOG_INFO(L"Enabling hardware " + deviceName + L"...");
                                if (device->Enable()) {
                                    LOG_INFO(L"Enabled hardware " + deviceName);
                                } else {
                                    LOG_WARNING(L"Could not enable hardware " + deviceName);
                                    MessageBoxW(NULL, wstring(L"Failed to enable " + deviceName + L".\r\nCheck the log to see if there are more details.").c_str(), L"LightFX Extender", MB_OK | MB_ICONERROR);
                                }
                            }
                        } else {
                            LOG_WARNING(L"Got result " + to_wstring(result) + L" from tray icon but couldn't find a device attached to that id");
                        }
                    } else {
                        LOG_WARNING(L"Got invalid result " + to_wstring(result) + L" from tray icon");
                    }
                } else if (lParam == NIN_BALLOONUSERCLICK) {
                    if (this->updateVersionUrl != L"") {
                        ShellExecuteW(NULL, L"open", this->updateVersionUrl.c_str(), NULL, NULL, SW_SHOWDEFAULT);
                    }
                }
            }
        }

    }
}

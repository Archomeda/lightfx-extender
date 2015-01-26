#include "TrayIcon.h"

// Standard includes
#include <algorithm>
#include <string>
#include <locale>
#include <codecvt>

// Windows includes
#include <CommCtrl.h>
#include <shellapi.h>
#include <strsafe.h>

// Project includes
#include "Utils/FileIO.h"
#include "Utils/Windows.h"
#include "resource.h"


#define WINDOW_CLASSNAME L"TrayIcon"
#define TRAYID 1
#define WM_TRAYICON WM_USER + 1

#define MENU_CONFFOLDER_NAME L"Open configuration folder..."

using namespace std;
using namespace lightfx::utils;
using namespace lightfx::devices;

namespace lightfx {

    bool TrayIcon::AddTrayIcon() {
        this->hModuleInstance = GetCurrentModule();
        this->trayIconWindowClass.lpfnWndProc = &TrayIcon::WndProc;
        this->trayIconWindowClass.hInstance = this->hModuleInstance;
        this->trayIconWindowClass.lpszClassName = WINDOW_CLASSNAME;
        this->trayIconWindowClass.cbWndExtra = sizeof(TrayIcon*);
        RegisterClassW(&this->trayIconWindowClass);
        this->hTrayIconWindow = CreateWindowExW(0, WINDOW_CLASSNAME, L"LightFX Tray Icon", 0, 0, 0, 0, 0, NULL, NULL, this->hModuleInstance, static_cast<LPVOID>(this));

        wstring fname, ext;
        wstring filename = GetProcessName(nullptr, nullptr, &fname, &ext);

        this->trayIconData = {};
        this->trayIconData.cbSize = sizeof(NOTIFYICONDATAW);
        this->trayIconData.hWnd = this->hTrayIconWindow;
        this->trayIconData.uID = TRAYID;
        this->trayIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        StringCchCopyW(this->trayIconData.szTip, ARRAYSIZE(this->trayIconData.szTip), (L"LightFX for " + fname + ext).c_str());
        this->trayIconData.uVersion = NOTIFYICON_VERSION_4;
        this->trayIconData.uCallbackMessage = WM_TRAYICON;

        // Not sure if taking the icon from an EXE file is desired by some companies,
        // as it might cause confusion to users if it's officially supported by those companies or not.
        // Therefore, it's disabled for now.
        //this->trayIconData.hIcon = ExtractIconW(GetModuleHandle(NULL), filename.c_str(), 0);
        if (this->trayIconData.hIcon == NULL) {
            // Fall back to our default (somewhat crappy) icon if the executable icon cannot be found
            this->trayIconData.hIcon = (HICON)LoadImageW(this->hModuleInstance, MAKEINTRESOURCEW(IDI_TRAYICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
        }

        return Shell_NotifyIconW(NIM_ADD, &this->trayIconData) == TRUE;
    }

    bool TrayIcon::RemoveTrayIcon() {
        bool result = Shell_NotifyIconW(NIM_DELETE, &this->trayIconData) == TRUE;
        DestroyWindow(this->hTrayIconWindow);
        this->hTrayIconWindow = NULL;
        UnregisterClassW(WINDOW_CLASSNAME, this->hModuleInstance);
        this->hModuleInstance = NULL;
        return result;
    }

    void TrayIcon::AttachDevice(shared_ptr<DeviceBase> device) {
        this->attachedDevices.push_back(device);
    }

    void TrayIcon::DetachDevice(shared_ptr<DeviceBase> device) {
        this->attachedDevices.erase(std::remove(this->attachedDevices.begin(), this->attachedDevices.end(), device), this->attachedDevices.end());
    }

    LRESULT CALLBACK TrayIcon::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_NCCREATE) {
            // Set extra window space to point to the this object
            CREATESTRUCT* pCs = reinterpret_cast<CREATESTRUCT*>(lParam);
            LPVOID pThis = pCs->lpCreateParams;
            SetWindowLongPtrW(hWnd, 0, reinterpret_cast<LONG_PTR>(pThis));
        }

        // Retrieve the pointer to this from the extra window space
        TrayIcon* that = reinterpret_cast<TrayIcon*>(GetWindowLongPtrW(hWnd, 0));

        switch (msg) {
        case WM_TRAYICON:
            that->TrayIconCallback(wParam, lParam);
            break;
        }
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    };

    void TrayIcon::TrayIconCallback(WPARAM wParam, LPARAM lParam) {
        if (wParam == TRAYID && lParam == WM_RBUTTONUP) {
            HMENU hMenu = CreatePopupMenu();

            size_t index;
            for (index = 1; index <= this->attachedDevices.size(); ++index) {
                string deviceName = "";
                unsigned char deviceType = 0;
                InsertMenuW(hMenu, index, this->attachedDevices[index - 1]->IsEnabled() ? MF_CHECKED : MF_UNCHECKED, index, this->attachedDevices[index - 1]->GetDeviceName().c_str());
            }
            InsertMenuW(hMenu, index, MF_SEPARATOR, 0, NULL);
            ++index;
            const UINT confFolderIndex = index;
            InsertMenuW(hMenu, confFolderIndex, 0, confFolderIndex, MENU_CONFFOLDER_NAME);

            POINT cursor;
            GetCursorPos(&cursor);
            SetForegroundWindow(this->hTrayIconWindow);
            UINT result = TrackPopupMenu(hMenu, TPM_RETURNCMD, cursor.x, cursor.y, 0, this->hTrayIconWindow, NULL);
            DestroyMenu(hMenu);

            if (result == confFolderIndex) {
                wstring folder;
                if (GetRoamingAppDataFolder(folder)) {
                    folder = folder + L"/" + APPDATA_FOLDER;
                    ShellExecuteW(NULL, L"explore", folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
                }
            } else if (result > 0 && result <= this->attachedDevices.size()) {
                size_t index = result - 1;
                if (this->attachedDevices[index]->IsEnabled()) {
                    this->attachedDevices[index]->DisableDevice();
                } else {
                    this->attachedDevices[index]->EnableDevice();
                }
            }
        }
    }

}

#include "DeviceManager.h"

// Standard includes
#include <locale>
#include <codecvt>

// Windows includes
#include <CommCtrl.h>
#include <shellapi.h>
#include <strsafe.h>

// Project includes
#include "Utils/FileIO.h"
#include "Utils/Log.h"
#include "resource.h"


using namespace std;
using namespace lightfx::devices;
using namespace lightfx::utils;

#pragma region Windows helper functions

extern "C" IMAGE_DOS_HEADER __ImageBase;

HMODULE GetCurrentModule() {
    return reinterpret_cast<HMODULE>(&__ImageBase);
}

bool CheckOS(DWORD majorVer, DWORD minorVer) {
    // Initialize the OSVERSIONINFOEX structure.
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = majorVer;
    osvi.dwMinorVersion = minorVer;

    // Initialize the condition mask.
    DWORDLONG dwlConditionMask = 0;
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

    // Perform the test.
    return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask) == TRUE;
}

#pragma endregion

namespace lightfx {

    DeviceManager::~DeviceManager() {
        this->Dispose();
        this->lightFXManager.~LightFX();
    }

    LightFX* DeviceManager::GetLightFXManager() {
        return &this->lightFXManager;
    }

    bool DeviceManager::Initialize() {
        if (!this->isInitialized) {
            Log(L"Connected to " + this->GetProcessName(nullptr, nullptr, nullptr, nullptr));

            // Config
            this->config.Load();

            // Logitech
            this->deviceLogitech = shared_ptr<DeviceLogitech>(new DeviceLogitech());
            this->deviceLogitech->SetRange(this->config.LogitechColorRangeOutMin, this->config.LogitechColorRangeOutMax, this->config.LogitechColorRangeInMin, this->config.LogitechColorRangeInMax);
            this->lightFXManager.AddDevice(this->deviceLogitech);
            if (this->config.LogitechEnabled) {
                this->EnableLogitechDevice();
            }

            // Lightpack
            this->deviceLightpack = shared_ptr<DeviceLightpack>(new DeviceLightpack(this->config.LightpackHost, this->config.LightpackPort, this->config.LightpackKey));
            this->lightFXManager.AddDevice(this->deviceLightpack);
            if (this->config.LightpackEnabled) {
                this->EnableLightpackDevice();
            }

            // Memory mapped file
            for (DeviceData devData : this->config.MemoryMappedFileDevices) {
                auto device = shared_ptr<DeviceMmf>(new DeviceMmf(devData.Description, devData.Type, devData.Lights));
                this->deviceMemoryMappedFile.push_back(device);
                this->lightFXManager.AddDevice(device);
            }
            if (this->config.MemoryMappedFileEnabled) {
                this->EnableMmfDevice();
            }

            this->AddTrayIcon();
            this->isInitialized = true;
        }
        return true;
    }

    bool DeviceManager::Dispose() {
        if (this->isInitialized) {
            this->RemoveTrayIcon();

            // Save before disabling stuff
            this->config.Save();

            this->DisableLogitechDevice();
            this->DisableLightpackDevice();
            this->DisableMmfDevice();

            this->isInitialized = false;
        }
        return true;
    }

    bool DeviceManager::EnableLogitechDevice() {
        if (!this->deviceLogitech->IsEnabled()) {
            this->config.LogitechEnabled = true;
            if (this->deviceLogitech->EnableDevice()) {
                Log("Logitech export enabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::DisableLogitechDevice() {
        if (this->deviceLogitech->IsEnabled()) {
            this->config.LogitechEnabled = false;
            if (this->deviceLogitech->DisableDevice()) {
                Log("Logitech export disabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::EnableLightpackDevice() {
        if (!this->deviceLightpack->IsEnabled()) {
            this->config.LightpackEnabled = true;
            if (this->deviceLightpack->EnableDevice()) {
                Log("Lightpack export enabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::DisableLightpackDevice() {
        if (this->deviceLightpack->IsEnabled()) {
            this->config.LightpackEnabled = false;
            if (this->deviceLightpack->DisableDevice()) {
                Log("Lightpack export disabled");
                return true;
            }
            return false;
        }
        return true;
    }

    bool DeviceManager::EnableMmfDevice() {
        if (!this->deviceMemoryMappedFileEnabled) {
            this->config.MemoryMappedFileEnabled = true;
            for (auto device : this->deviceMemoryMappedFile) {
                device->EnableDevice();
            }
            Log("Memory Mapped File passthrough enabled");
            this->deviceMemoryMappedFileEnabled = true;
        }
        return true;
    }

    bool DeviceManager::DisableMmfDevice() {
        if (this->deviceMemoryMappedFileEnabled) {
            this->config.MemoryMappedFileEnabled = false;
            for (auto device : this->deviceMemoryMappedFile) {
                device->DisableDevice();
            }
            Log("Memory Mapped File passthrough disabled");
            this->deviceMemoryMappedFileEnabled = false;
        }
        return true;
    }


    wstring DeviceManager::GetProcessName(wstring* drive, wstring* dir, wstring* fname, wstring* ext) {
        wchar_t szFileName[MAX_PATH];
        GetModuleFileNameW(NULL, szFileName, MAX_PATH);
        wchar_t cdrive[_MAX_DRIVE];
        wchar_t cdir[_MAX_DIR];
        wchar_t cfname[_MAX_FNAME];
        wchar_t cext[_MAX_EXT];
        _wsplitpath_s(szFileName, cdrive, sizeof(cdrive), cdir, sizeof(cdir), cfname, sizeof(cfname), cext, sizeof(cext));

        if (drive != nullptr) {
            *drive = wstring(cdrive);
        }
        if (dir != nullptr) {
            *dir = wstring(cdir);
        }
        if (fname != nullptr) {
            *fname = wstring(cfname);
        }
        if (ext != nullptr) {
            *ext = wstring(cext);
        }

        return wstring(szFileName);
    }


#pragma region Tray Icon

#define ISWIN7ORLATER CheckOS(6, 1)

#define WINDOW_CLASSNAME "TrayIcon"
#define TRAYID 1
#define WM_TRAYICON WM_USER + 1

#define MENU_LOGITECHENABLED 1
#define MENU_LOGITECHENABLED_NAME "Logitech devices"
#define MENU_LIGHTPACKENABLED 2
#define MENU_LIGHTPACKENABLED_NAME "Lightpack device"
#define MENU_MMFENABLED 3
#define MENU_MMFENABLED_NAME "Memory mapped file passthrough"
#define MENU_CONFFOLDER 100
#define MENU_CONFFOLDER_NAME "Open configuration folder..."

    bool DeviceManager::AddTrayIcon() {
        this->hModuleInstance = GetCurrentModule();
        this->trayIconWindowClass.lpfnWndProc = &DeviceManager::WndProc;
        this->trayIconWindowClass.hInstance = this->hModuleInstance;
        this->trayIconWindowClass.lpszClassName = WINDOW_CLASSNAME;
        this->trayIconWindowClass.cbWndExtra = sizeof(DeviceManager*);
        RegisterClass(&this->trayIconWindowClass);
        this->hTrayIconWindow = CreateWindowEx(0, WINDOW_CLASSNAME, "LightFX Tray Icon", 0, 0, 0, 0, 0, NULL, NULL, this->hModuleInstance, static_cast<LPVOID>(this));

        wstring fname, ext;
        wstring filename = this->GetProcessName(nullptr, nullptr, &fname, &ext);

        this->trayIconData = {};
        this->trayIconData.cbSize = sizeof(NOTIFYICONDATA);
        this->trayIconData.hWnd = this->hTrayIconWindow;
        this->trayIconData.uID = TRAYID;
        this->trayIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_GUID;
        StringCchCopy(this->trayIconData.szTip, ARRAYSIZE(this->trayIconData.szTip), ("LightFX for " + wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(fname + ext)).c_str());
        this->trayIconData.uVersion = NOTIFYICON_VERSION_4;
        this->trayIconData.uCallbackMessage = WM_TRAYICON;

        if (ISWIN7ORLATER) {
            // {1512AC64-855C-4119-B965-41867A9EFF42}
            static const GUID guid = { 0x1512ac64, 0x855c, 0x4119, { 0xb9, 0x65, 0x41, 0x86, 0x7a, 0x9e, 0xff, 0x42 } };
            this->trayIconData.guidItem = guid;
        }

        // Not sure if taking the icon from an EXE file is desired by some companies,
        // as it might cause confusion to users if it's officially supported by those companies or not.
        // Therefore, it's disabled for now.
        //this->trayIconData.hIcon = ExtractIconW(GetModuleHandle(NULL), filename.c_str(), 0);
        if (this->trayIconData.hIcon == NULL) {
            // Fall back to our default (somewhat crappy) icon if the executable icon cannot be found
            this->trayIconData.hIcon = (HICON)LoadImage(this->hModuleInstance, MAKEINTRESOURCE(IDI_TRAYICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
        }

        return Shell_NotifyIcon(NIM_ADD, &this->trayIconData) == TRUE;
    }

    bool DeviceManager::RemoveTrayIcon() {
        bool result = Shell_NotifyIcon(NIM_DELETE, &this->trayIconData) == TRUE;
        DestroyWindow(this->hTrayIconWindow);
        this->hTrayIconWindow = NULL;
        UnregisterClass(WINDOW_CLASSNAME, this->hModuleInstance);
        this->hModuleInstance = NULL;
        return result;
    }

    LRESULT CALLBACK DeviceManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_NCCREATE) {
            // Set extra window space to point to the this object
            CREATESTRUCT* pCs = reinterpret_cast<CREATESTRUCT*>(lParam);
            LPVOID pThis = pCs->lpCreateParams;
            SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(pThis));
        }

        // Retrieve the pointer to this from the extra window space
        DeviceManager* that = reinterpret_cast<DeviceManager*>(GetWindowLongPtr(hWnd, 0));

        switch (msg) {
        case WM_TRAYICON:
            that->TrayIconCallback(wParam, lParam);
            break;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    };

    void DeviceManager::TrayIconCallback(WPARAM wParam, LPARAM lParam) {
        if (wParam == TRAYID && lParam == WM_RBUTTONUP) {
            HMENU hMenu = CreatePopupMenu();

            InsertMenu(hMenu, MENU_LOGITECHENABLED, this->deviceLogitech->IsEnabled() ? MF_CHECKED : 0, MENU_LOGITECHENABLED, MENU_LOGITECHENABLED_NAME);
            InsertMenu(hMenu, MENU_LIGHTPACKENABLED, this->deviceLightpack->IsEnabled() ? MF_CHECKED : 0, MENU_LIGHTPACKENABLED, MENU_LIGHTPACKENABLED_NAME);
            InsertMenu(hMenu, MENU_MMFENABLED, this->deviceMemoryMappedFileEnabled ? MF_CHECKED : 0, MENU_MMFENABLED, MENU_MMFENABLED_NAME);
            InsertMenu(hMenu, MENU_CONFFOLDER - 1, MF_SEPARATOR, 0, NULL);
            InsertMenu(hMenu, MENU_CONFFOLDER, 0, MENU_CONFFOLDER, MENU_CONFFOLDER_NAME);

            POINT cursor;
            GetCursorPos(&cursor);
            SetForegroundWindow(this->hTrayIconWindow);
            UINT result = TrackPopupMenu(hMenu, TPM_RETURNCMD, cursor.x, cursor.y, 0, this->hTrayIconWindow, NULL);
            DestroyMenu(hMenu);

            switch (result) {
            case MENU_LOGITECHENABLED:
                if (this->deviceLogitech->IsEnabled()) {
                    this->DisableLogitechDevice();
                } else {
                    this->EnableLogitechDevice();
                }
                break;

            case MENU_LIGHTPACKENABLED:
                if (this->deviceLightpack->IsEnabled()) {
                    this->DisableLightpackDevice();
                } else {
                    this->EnableLightpackDevice();
                }
                break;

            case MENU_MMFENABLED:
                if (this->deviceMemoryMappedFileEnabled) {
                    this->DisableMmfDevice();
                } else {
                    this->EnableMmfDevice();
                }
                break;

            case MENU_CONFFOLDER:
                wstring folder;
                if (GetRoamingAppDataFolder(folder)) {
                    folder = folder + L"/" + APPDATA_FOLDER;
                    ShellExecuteW(NULL, L"explore", folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
                }
                break;
            }

        }
    }

#pragma endregion

}

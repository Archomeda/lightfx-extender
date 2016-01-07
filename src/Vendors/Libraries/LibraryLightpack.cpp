#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "LibraryLightpack.h"

// Standard includes
#include <limits>
#include <locale>
#include <regex>

// Windows includes
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// Project includes
#include "../../Utils/Log.h"
#include "../../Utils/String.h"

#define API_BUFFLEN 8192


using namespace std;
using namespace lightfx;
using namespace lightfx::utils;

namespace lightfx {
    namespace vendors {
        namespace libraries {

            LFXE_API LibraryLightpack::~LibraryLightpack() {
                this->ReleaseLibrary();
            }

            LFXE_API bool LibraryLightpack::IsLibraryAvailable() const {
                // This is tricky, we have to actually connect and see if the socket is working correctly or not
                if (!this->isInitialized) {
                    bool result = this->Connect();
                    this->Disconnect();
                    return result;
                }
                return true;
            }

            LFXE_API bool LibraryLightpack::InitializeLibrary() {
                if (!this->isInitialized) {
                    LOG_INFO(L"Initializing Lightpack library...");
                    this->isInitialized = this->Connect();
                    if (this->isInitialized) {
                        LOG_INFO(L"Lightpack library initialized");
                    } else {
                        LOG_WARNING(L"Could not initialize Lightpack library");
                    }
                }
                return this->isInitialized;
            }

            LFXE_API bool LibraryLightpack::ReleaseLibrary() {
                if (this->isInitialized) {
                    LOG_INFO(L"Releasing Lightpack library...");
                    this->isInitialized = !this->Disconnect();
                    if (!this->isInitialized) {
                        LOG_INFO(L"Lightpack library released");
                    } else {
                        LOG_WARNING(L"Could not release Lightpack library");
                    }
                }
                return !this->isInitialized;
            }

            LFXE_API Version LibraryLightpack::GetLibraryVerion() const {
                // Sadly, we have to initialize before we can get the API version
                // So if we aren't initialized, let's do that first and shut down afterwards
                if (this->apiVersion != Version()) {
                    return this->apiVersion;
                }

                if (!this->isInitialized) {
                    bool result = this->Connect();
                    this->Disconnect();
                    if (!result) {
                        return{};
                    }
                }

                // The API version is automatically parsed when connecting
                return this->apiVersion;
            }

            LFXE_API bool LibraryLightpack::Connect() const {
                LOG_DEBUG(L"Connecting to Lightpack API socket...");

                WSADATA wsaData;
                if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                    LOG_WARNING(L"WSAStartup failed");
                    return false;
                }

                struct addrinfoW *addrInfo = NULL, hints;

                ZeroMemory(&hints, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;

                if (GetAddrInfoW(this->hostname.c_str(), this->port.c_str(), &hints, &addrInfo) != 0) {
                    LOG_WARNING(L"GetAddrInfo failed");
                    WSACleanup();
                    return false;
                }

                this->lightpackSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
                if (this->lightpackSocket == INVALID_SOCKET) {
                    LOG_WARNING(L"Invalid scoket");
                    FreeAddrInfoW(addrInfo);
                    WSACleanup();
                    return false;
                }

                if (connect(this->lightpackSocket, addrInfo->ai_addr, (int)addrInfo->ai_addrlen) == SOCKET_ERROR) {
                    LOG_WARNING(L"Connection error");
                    closesocket(this->lightpackSocket);
                    this->lightpackSocket = INVALID_SOCKET;
                    FreeAddrInfoW(addrInfo);
                    WSACleanup();
                    return false;
                }

                wstring greeting = this->Receive();
                wregex re(L"Lightpack API v(\\d+\\.\\d+(\\.\\d+(\\.\\d+)?)?).*");
                wsmatch match;
                if (regex_search(greeting, match, re) && match.size() > 1) {
                    this->apiVersion = Version::FromString(match.str(1));
                }

                return true;
            }

            LFXE_API bool LibraryLightpack::Disconnect() const {
                LOG_DEBUG(L"Disconnecting from Lightpack API socket...");

                this->Lp_Unlock();
                shutdown(this->lightpackSocket, SD_SEND);

                return true;
            }

            LFXE_API bool LibraryLightpack::Send(const wstring& command) const {
                LOG_DEBUG(command);
                wstring str = command + L"\n";
                if (str.length() > static_cast<size_t>(numeric_limits<int>::max())) {
                    LOG_ERROR(L"Command string length too big");
                    return false;
                }
                if (send(this->lightpackSocket, wstring_to_string(str).c_str(), static_cast<int>(str.length()), 0) == SOCKET_ERROR) {
                    LOG_ERROR(L"Socket error");
                    return false;
                }
                return true;
            }

            LFXE_API wstring LibraryLightpack::Receive() const {
                wstring resultw;
                char buff[API_BUFFLEN];
                int result = recv(this->lightpackSocket, buff, API_BUFFLEN, 0);
                if (result > 0) {
                    string result(buff);
                    resultw = string_to_wstring(result.substr(0, result.find("\n") - 1));
                } else if (result == 0) {
                    // Connection closed
                } else {
                    // Error
                }
                LOG_DEBUG(resultw);
                return resultw;
            }

            LFXE_API wstring LibraryLightpack::StripLeadingResult(const wstring& str) const {
                size_t pos = str.find(L":");
                if (pos != wstring::npos) {
                    return str.substr(pos + 1);
                }
                return str;
            }

            LFXE_API vector<wstring> LibraryLightpack::SplitList(const wstring& str, const char delim) const {
                return split(str, delim);
            }

            LFXE_API LightpackStatus LibraryLightpack::ParseStatus(const wstring& str) const {
                wstring status = this->StripLeadingResult(str);
                if (status == L"ok" || status == L"success") {
                    return LightpackStatus::LightpackSuccess;
                } else if (status == L"fail") {
                    return LightpackStatus::LightpackFail;
                } else if (status == L"busy") {
                    return LightpackStatus::LightpackBusy;
                } else if (status == L"idle") {
                    return LightpackStatus::LightpackIdle;
                } else if (status == L"not locked") {
                    return LightpackStatus::LightpackNotLocked;
                } else if (status == L"device error") {
                    return LightpackStatus::LightpackDeviceError;
                } else if (status == L"on") {
                    return LightpackStatus::LightpackOn;
                } else if (status == L"off") {
                    return LightpackStatus::LightpackOff;
                } else if (status == L"unknown") {
                    return LightpackStatus::LightpackUnknown;
                } else if (status == L"unknown command") {
                    return LightpackStatus::LightpackUnknownCommand;
                } else {
                    return LightpackStatus::LightpackUnknown;
                }
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_Authorize(const wstring& key) const {
                if (!this->Send(L"apikey:" + key)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_Lock() const {
                if (!this->Send(L"lock")) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_Unlock() const {
                if (!this->Send(L"unlock")) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_GetStatus() const {
                if (!this->Send(L"getstatus")) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_GetStatusApi() const {
                if (!this->Send(L"getstatusapi")) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API wstring LibraryLightpack::Lp_GetCurrentProfile() const {
                if (!this->Send(L"getprofile")) {
                    LOG_ERROR(L"Error while sending command");
                    return L"";
                }
                return this->StripLeadingResult(this->Receive());
            }

            LFXE_API vector<wstring> LibraryLightpack::Lp_GetProfiles() const {
                if (!this->Send(L"getprofiles")) {
                    LOG_ERROR(L"Error while sending command");
                    return{};
                }
                return this->SplitList(this->StripLeadingResult(this->Receive()));
            }

            LFXE_API int LibraryLightpack::Lp_GetCountLeds() const {
                if (!this->Send(L"getcountleds")) {
                    LOG_ERROR(L"Error while sending command");
                    return 0;
                }
                try {
                    return stoi(this->StripLeadingResult(this->Receive()));
                } catch (...) {
                    return 0;
                }
            }

            LFXE_API vector<LightpackLed> LibraryLightpack::Lp_GetLeds() const {
                if (!this->Send(L"getleds")) {
                    LOG_ERROR(L"Error while sending command");
                    return{};
                }
                auto listStr = this->SplitList(this->StripLeadingResult(this->Receive()));
                vector<LightpackLed> list = {};
                for (auto ledStr : listStr) {
                    LightpackLed led = {};
                    auto ledSplit = this->SplitList(ledStr, '-');
                    try {
                        led.index = stoi(ledSplit[0]);
                        ledSplit = this->SplitList(ledSplit[1], ',');
                        led.x = stoi(ledSplit[0]);
                        led.y = stoi(ledSplit[1]);
                        led.width = stoi(ledSplit[2]);
                        led.height = stoi(ledSplit[3]);
                    } catch (...) {}
                    list.push_back(led);
                }
                return list;
            }

            LFXE_API vector<LightpackColor> LibraryLightpack::Lp_GetColors() const {
                if (!this->Send(L"getcolors")) {
                    LOG_ERROR(L"Error while sending command");
                    return{};
                }
                auto listStr = this->SplitList(this->StripLeadingResult(this->Receive()));
                vector<LightpackColor> list = {};
                for (auto colorStr : listStr) {
                    LightpackColor color = {};
                    auto colorSplit = this->SplitList(colorStr, '-');
                    try {
                        color.index = stoi(colorSplit[0]);
                        colorSplit = this->SplitList(colorSplit[1], ',');
                        color.red = stoi(colorSplit[0]);
                        color.green = stoi(colorSplit[1]);
                        color.blue = stoi(colorSplit[2]);
                    } catch (...) {}
                    list.push_back(color);
                }
                return list;
            }

            LFXE_API double LibraryLightpack::Lp_GetFps() const {
                if (!this->Send(L"getfps")) {
                    LOG_ERROR(L"Error while sending command");
                    return{};
                }

                char* locale = setlocale(LC_NUMERIC, NULL);
                setlocale(LC_NUMERIC, "en-US");
                double fps = 0;
                try {
                    fps = stod(this->StripLeadingResult(this->Receive()));
                } catch (...) {}
                setlocale(LC_NUMERIC, locale);
                return fps;
            }

            LFXE_API LightpackScreen LibraryLightpack::Lp_GetScreenSize() const {
                if (!this->Send(L"getscreensize")) {
                    LOG_ERROR(L"Error while sending command");
                    return{};
                }
                auto screenList = this->SplitList(this->StripLeadingResult(this->Receive()), ',');
                LightpackScreen screen = {};
                try {
                    screen.x = stoi(screenList[0]);
                    screen.y = stoi(screenList[1]);
                    screen.width = stoi(screenList[2]);
                    screen.height = stoi(screenList[3]);
                } catch (...) {}
                vector<LightpackLed> list = {};
                return screen;
            }

            LFXE_API wstring LibraryLightpack::Lp_GetMode() const {
                if (!this->Send(L"getmode")) {
                    LOG_ERROR(L"Error while sending command");
                    return L"";
                }
                return this->StripLeadingResult(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetColors(const vector<LightpackColor>& colors) const {
                wstring command = L"setcolor:";
                for (LightpackColor color : colors) {
                    command += to_wstring(color.index) + L"-" + to_wstring(color.red) + L"," + to_wstring(color.green) + L"," + to_wstring(color.blue) + L";";
                }
                if (!this->Send(command)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetLeds(const vector<LightpackLed>& leds) const {
                wstring command = L"setleds:";
                for (LightpackLed led : leds) {
                    command += to_wstring(led.index) + L"-" + to_wstring(led.x) + L"," + to_wstring(led.y) + L"," + to_wstring(led.width) + L"," + to_wstring(led.height) + L";";
                }
                if (!this->Send(command)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetGamma(const double gamma) const {
                if (!this->Send(L"setgamma:" + to_wstring(gamma))) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetBrightness(const int brightness) const {
                if (!this->Send(L"setbrightness:" + to_wstring(brightness))) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetSmooth(const int smooth) const {
                if (!this->Send(L"setsmooth:" + to_wstring(smooth))) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetCurrentProfile(const wstring& profile) const {
                if (!this->Send(L"setprofile:" + profile)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_NewProfile(const wstring& profile) const {
                if (!this->Send(L"newprofile:" + profile)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_DeleteProfile(const wstring& profile) const {
                if (!this->Send(L"deleteprofile:" + profile)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetStatus(const bool active) const {
                wstring status = active ? L"on" : L"off";
                if (!this->Send(L"setstatus:" + status)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API LightpackStatus LibraryLightpack::Lp_SetMode(const wstring& mode) const {
                if (!this->Send(L"setmode:" + mode)) {
                    LOG_ERROR(L"Error while sending command");
                    return LightpackStatus::LightpackFail;
                }
                return this->ParseStatus(this->Receive());
            }

            LFXE_API wstring LibraryLightpack::LightpackStatusToString(const LightpackStatus status) const {
                switch (status) {
                case LightpackSuccess:
                    return L"LightpackSuccess";
                case LightpackFail:
                    return L"LightpackFail";
                case LightpackBusy:
                    return L"LightpackBusy";
                case LightpackIdle:
                    return L"LightpackIdle";
                case LightpackNotLocked:
                    return L"LightpackNotLocked";
                case LightpackDeviceError:
                    return L"LightpackDeviceError";
                case LightpackOn:
                    return L"LightpackOn";
                case LightpackOff:
                    return L"LightpackOff";
                case LightpackError:
                    return L"LightpackError";
                case LightpackUnknown:
                    return L"LightpackUnknown";
                case LightpackUnknownCommand:
                    return L"LightpackUnknownCommand";
                default:
                    return L"Unknown error " + to_wstring(status);
                }
            }

       }
    }
}

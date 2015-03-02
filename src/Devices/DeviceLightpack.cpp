#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceLightpack.h"

// Windows includes
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// Project includes
#include "../LightFXExtender.h"
#include "../Managers/LogManager.h"
#include "../Utils/String.h"


#define LOG(logLevel, line) if (this->GetManager() != nullptr) { this->GetManager()->GetLightFXExtender()->GetLogManager()->Log(logLevel, wstring(L"Device ") + this->GetDeviceName() + L" - " + line); }

using namespace std;
using namespace lightfx::managers;
using namespace lightfx::utils;

#define API_BUFFLEN 8192

namespace lightfx {
    namespace devices {

        DeviceLightpack::DeviceLightpack(const wstring& hostname, const wstring& port, const wstring& key) {
            this->hostname = hostname;
            this->port = port;
            this->key = key;
        }

        bool DeviceLightpack::Initialize() {
            if (!this->IsInitialized()) {
                if (Device::Initialize()) {
                    if (this->ConnectAPI()) {
                        this->SetNumberOfLights(this->GetCountLeds());
                        auto leds = this->GetLeds();
                        LightpackScreen screen = this->GetScreenSize();
                        double divider = max(screen.width - screen.x, screen.height - screen.y) / 100.0;
                        for (size_t i = 0; i < this->GetNumberOfLights(); ++i) {
                            LightData light;
                            light.Name = to_wstring(leds[i].index);
                            int posX = int(((leds[i].x - screen.x) + (leds[i].width / 2)) / divider);
                            int posY = int(((screen.height - leds[i].y - screen.y) + (leds[i].height / 2)) / divider);
                            light.Position = { posX, posY, 0 };
                            this->SetLightData(i, light);
                        }
                        return true;
                    }
                }
            }
            return false;
        }

        bool DeviceLightpack::Release() {
            if (this->IsInitialized()) {
                if (Device::Release()) {
                    this->DisconnectAPI();
                    return true;
                }
            }
            return false;
        }

        bool DeviceLightpack::PushColorToDevice() {
            vector<LightpackColor> newLights = {};
            for (size_t i = 0; i < this->GetNumberOfLights(); ++i) {
                LightColor color = this->CurrentLightAction.GetCurrentColor(i);
                double brightness = color.brightness / 255.0;
                int red = int(color.red * brightness);
                int green = int(color.green * brightness);
                int blue = int(color.blue * brightness);
                newLights.push_back({ i + 1, red, green, blue });
            }
            this->SetColors(newLights);
            return true;
        }


        bool DeviceLightpack::ConnectAPI() {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                // WSAStartup failed
                LOG(LogLevel::Error, L"WSAStartup failed");
                return false;
            }

            struct addrinfoW *result = NULL, hints;

            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            if (GetAddrInfoW(this->hostname.c_str(), this->port.c_str(), &hints, &result) != 0) {
                // GetAddrInfoW failed
                LOG(LogLevel::Error, L"GetAddrInfoW failed");
                WSACleanup();
                return false;
            }

            this->lightpackSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (this->lightpackSocket == INVALID_SOCKET) {
                // Invalid socket
                LOG(LogLevel::Error, L"Invalid socket");
                FreeAddrInfoW(result);
                WSACleanup();
                return false;
            }

            if (connect(this->lightpackSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
                // Connection error
                LOG(LogLevel::Error, L"Connection error");
                closesocket(this->lightpackSocket);
                this->lightpackSocket = INVALID_SOCKET;
                FreeAddrInfoW(result);
                WSACleanup();
                return false;
            }

            FreeAddrInfoW(result);
            this->ReceiveAPI();
            if (this->ApiKey(this->key) == LightpackSuccess) {
                if (this->Lock() != LightpackSuccess) {
                    this->DisconnectAPI();
                    return false;
                }
                return true;
            } else {
                return false;
            }
        }

        bool DeviceLightpack::DisconnectAPI() {
            this->Unlock();

            if (shutdown(this->lightpackSocket, SD_SEND) == SOCKET_ERROR) {
                // Disconnection error
                closesocket(this->lightpackSocket);
                WSACleanup();
                return false;
            }

            closesocket(this->lightpackSocket);
            WSACleanup();
            return true;
        }

        bool DeviceLightpack::SendAPI(const wstring& cmd) {
            wstring s = cmd + L"\n";
            int result = send(this->lightpackSocket, wstring_to_string(s).c_str(), s.length(), 0);
            if (result == SOCKET_ERROR) {
                closesocket(this->lightpackSocket);
                WSACleanup();
                return false;
            }
            return true;
        }

        wstring DeviceLightpack::ReceiveAPI() {
            char buff[API_BUFFLEN];
            int result = recv(this->lightpackSocket, buff, API_BUFFLEN, 0);
            if (result > 0) {
                string result = string(buff);
                result = result.substr(0, result.find("\n") - 1);
                return string_to_wstring(result);
            } else if (result == 0) {
                // Connection closed
                return L"";
            } else {
                // Error
                return L"";
            }
        }

        LightpackStatus DeviceLightpack::ApiKey(const wstring& key) {
            this->SendAPI(L"apikey:" + key);
            wstring result = this->ReceiveAPI();
            if (result == L"ok") {
                return LightpackSuccess;
            } else if (result == L"fail") {
                return LightpackFail;
            } else {
                return LightpackError;
            }
        }

        LightpackStatus DeviceLightpack::Lock() {
            this->SendAPI(L"lock");
            wstring result = this->ReceiveAPI();
            if (result == L"lock:success") {
                return LightpackSuccess;
            } else if (result == L"lock:busy") {
                return LightpackBusy;
            } else {
                return LightpackError;
            }
        }

        LightpackStatus DeviceLightpack::Unlock() {
            this->SendAPI(L"unlock");
            wstring result = this->ReceiveAPI();
            if (result == L"unlock:success") {
                return LightpackSuccess;
            } else if (result == L"unlock:not locked") {
                return LightpackNotLocked;
            } else {
                return LightpackError;
            }
        }

        int DeviceLightpack::GetCountLeds() {
            this->SendAPI(L"getcountleds");
            wstring result = this->ReceiveAPI();
            size_t pos = result.find(L":");
            if (pos != string::npos) {
                result.erase(0, pos + 1);
                return stoi(result);
            } else {
                return 0;
            }
        }

        vector<LightpackLed> DeviceLightpack::GetLeds() {
            vector<LightpackLed> v;
            this->SendAPI(L"getleds");
            wstring result = this->ReceiveAPI();

            // Find delimiter ':' first to seperate the result from the used command
            size_t pos = result.find(L":");
            if (pos != wstring::npos) {
                result.erase(0, pos + 1);
                // Repeat until end of string
                while (pos != wstring::npos) {
                    // Find delimter '-' to get the LED index
                    pos = result.find(L"-");
                    if (pos != wstring::npos) {
                        LightpackLed led;
                        wstring subresult = result.substr(0, pos);
                        result.erase(0, pos + 1);
                        led.index = stoi(subresult);
                        // Find 3x delimiter ',' to get the x, y and width of the captured area
                        for (int i = 0; i < 3; ++i) {
                            pos = result.find(L",");
                            if (pos != string::npos) {
                                subresult = result.substr(0, pos);
                                result.erase(0, pos + 1);
                                switch (i)
                                {
                                case 0:
                                    led.x = stoi(subresult);
                                    break;
                                case 1:
                                    led.y = stoi(subresult);
                                    break;
                                case 2:
                                    led.width = stoi(subresult);
                                    break;
                                }
                            }
                        }
                        // Find last delimiter ';' to get the height of the captured area
                        pos = result.find(L";");
                        if (pos != string::npos) {
                            subresult = result.substr(0, pos);
                            result.erase(0, pos + 1);
                            led.height = stoi(subresult);
                        }
                        v.push_back(led);
                    }
                }
            }
            return v;
        }

        LightpackScreen DeviceLightpack::GetScreenSize() {
            // Command is not working properly, so determine screen size by the LED positions
            auto leds = this->GetLeds();
            LightpackScreen result = {};
            for (auto led : leds) {
                result.width = max(result.width, led.x + led.width);
                result.height = max(result.height, led.y + led.height);
            }

            return result;

            /*
            LightpackScreen screen = {};
            this->SendAPI(L"getleds");
            wstring result = this->ReceiveAPI();

            // Find delimiter ':' first to seperate the result from the used command
            size_t pos = result.find(L":");
            if (pos != string::npos) {
                result.erase(0, pos + 1);
                // Find delimter ',' to seperate x, y and width
                for (int i = 0; i < 3; ++i) {
                    pos = result.find(L",");
                    if (pos != string::npos) {
                        wstring subresult = result.substr(0, pos);
                        result.erase(0, pos + 1);
                        switch (i) {
                        case 0:
                            screen.x = stoi(subresult);
                            break;
                        case 1:
                            screen.y = stoi(subresult);
                            break;
                        case 2:
                            screen.width = stoi(subresult);
                            break;
                        }
                    }
                }
                // Last bit is the height
                screen.height = stoi(result);
            }
            return screen;
            */
        }

        LightpackStatus DeviceLightpack::SetColor(const int index, const int red, const int green, const int blue) {
            return this->SetColors({ index }, { red }, { green }, { blue });
        }

        LightpackStatus DeviceLightpack::SetColor(const LightpackColor& color) {
            return this->SetColors({ color });
        }

        LightpackStatus DeviceLightpack::SetColors(const vector<int>& indices, const vector<int>& red, const vector<int>& green, const vector<int>& blue) {
            wstring cmd = L"setcolor:";
            for (size_t i = 0; i < indices.size(); ++i) {
                cmd += to_wstring(indices[i]) + L"-" + to_wstring(red[i]) + L"," + to_wstring(green[i]) + L"," + to_wstring(blue[i]) + L";";
            }
            this->SendAPI(cmd);
            wstring result = this->ReceiveAPI();
            if (result == L"ok") {
                return LightpackSuccess;
            } else if (result == L"busy") {
                return LightpackBusy;
            } else if (result == L"not locked") {
                return LightpackNotLocked;
            } else {
                return LightpackError;
            }
        }

        LightpackStatus DeviceLightpack::SetColors(const vector<LightpackColor>& colors) {
            wstring cmd = L"setcolor:";
            for (size_t i = 0; i < colors.size(); ++i) {
                cmd += to_wstring(colors[i].index) + L"-" + to_wstring(colors[i].red) + L"," + to_wstring(colors[i].green) + L"," + to_wstring(colors[i].blue) + L";";
            }
            this->SendAPI(cmd);
            wstring result = this->ReceiveAPI();
            if (result == L"ok") {
                return LightpackSuccess;
            } else if (result == L"busy") {
                return LightpackBusy;
            } else if (result == L"not locked") {
                return LightpackNotLocked;
            } else {
                return LightpackError;
            }
        }

    }
}

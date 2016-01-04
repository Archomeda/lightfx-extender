#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceLightpack.h"

// Standard includes
#include <limits>

// Windows includes
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"
#include "../Utils/String.h"


using namespace std;
using namespace lightfx::managers;
using namespace lightfx::timelines;
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
                    // Just do an initial pass to check how many LEDs there are available
                    if (this->ConnectAPI()) {
                        int countLeds = this->GetCountLeds();
                        LOG_DEBUG(to_wstring(countLeds) + L" LEDs available");
                        this->SetNumberOfLights(countLeds);
                        auto leds = this->GetLeds();
                        LightpackScreen screen = this->GetScreenSize();
                        double divider = max(screen.width - screen.x, screen.height - screen.y) / 100.0;
                        for (size_t i = 0; i < this->GetNumberOfLights(); ++i) {
                            LightData light;
                            light.Name = to_wstring(leds[i].index);
                            int posX = int(((leds[i].x - screen.x) + (leds[i].width / 2)) / divider);
                            int posY = int(((screen.height - leds[i].y - screen.y) + (leds[i].height / 2)) / divider);
                            light.Position = { posX, posY, 0 };
                            LOG_DEBUG(L"Get LED " + to_wstring(i) + L" pos: (" + to_wstring(posX) + L"," + to_wstring(posY) + L")");
                            this->SetLightData(i, light);
                        }

                        this->Reset();
                        this->DisconnectAPI();
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
            return true;
        }

        bool DeviceLightpack::Enable() {
            if (!this->IsEnabled()) {
                if (Device::Enable()) {
                    if (this->ConnectAPI()) {
                        this->Reset();
                    }
                } else {
                    return false;
                }
            }
            return true;
        }

        bool DeviceLightpack::Disable() {
            if (this->IsEnabled()) {
                if (Device::Disable()) {
                    this->DisconnectAPI();
                } else {
                    return false;
                }
            }
            return true;
        }

        bool DeviceLightpack::PushColorToDevice(const vector<LightColor>& colors) {
            vector<LightpackColor> newLights = {};
            for (size_t i = 0; i < colors.size(); ++i) {
                double brightness = colors[i].brightness / 255.0;
                int red = int(colors[i].red * brightness);
                int green = int(colors[i].green * brightness);
                int blue = int(colors[i].blue * brightness);
                newLights.push_back({ i + 1, red, green, blue });
            }
            this->SetColors(newLights);
            return true;
        }


        bool DeviceLightpack::ConnectAPI() {
            LOG_DEBUG(L"Connecting with API");

            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                LOG_ERROR(L"WSAStartup failed");
                return false;
            }

            struct addrinfoW *result = NULL, hints;

            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            if (GetAddrInfoW(this->hostname.c_str(), this->port.c_str(), &hints, &result) != 0) {
                LOG_ERROR(L"GetAddrInfoW failed");
                WSACleanup();
                return false;
            }

            this->lightpackSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (this->lightpackSocket == INVALID_SOCKET) {
                LOG_ERROR(L"Invalid socket");
                FreeAddrInfoW(result);
                WSACleanup();
                return false;
            }

            if (connect(this->lightpackSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
                LOG_ERROR(L"Connection error");
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
            LOG_DEBUG(L"Disconnecting from API");

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
            LOG_DEBUG(L"Send: " + cmd + L"");

            wstring s = cmd + L"\n";
            if (s.length() > (size_t)numeric_limits<int>::max()) {
                return false;
            }

            int result = send(this->lightpackSocket, wstring_to_string(s).c_str(), static_cast<int>(s.length()), 0);
            if (result == SOCKET_ERROR) {
                closesocket(this->lightpackSocket);
                WSACleanup();
                return false;
            }
            return true;
        }

        wstring DeviceLightpack::ReceiveAPI() {
            wstring wresult = L"";
            char buff[API_BUFFLEN];
            int result = recv(this->lightpackSocket, buff, API_BUFFLEN, 0);
            if (result > 0) {
                string result = string(buff);
                wresult = string_to_wstring(result.substr(0, result.find("\n") - 1));
            } else if (result == 0) {
                // Connection closed
            } else {
                // Error
            }

            LOG_DEBUG(L"Receive: " + wresult);
            return wresult;
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

            LOG_DEBUG(L"Screen size: (" + to_wstring(result.x) + L"," + to_wstring(result.y) + L"," + to_wstring(result.width) + L"," + to_wstring(result.height) + L")");
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

        LightpackStatus DeviceLightpack::SetColor(const size_t index, const int red, const int green, const int blue) {
            return this->SetColors({ index }, { red }, { green }, { blue });
        }

        LightpackStatus DeviceLightpack::SetColor(const LightpackColor& color) {
            return this->SetColors({ color });
        }

        LightpackStatus DeviceLightpack::SetColors(const vector<size_t>& indices, const vector<int>& red, const vector<int>& green, const vector<int>& blue) {
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

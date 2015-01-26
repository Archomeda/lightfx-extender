#include "DeviceLightpack.h"

// Windows includes
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// Project includes
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::utils;

#define DEVICENAME L"Lightpack"
#define DEVICETYPE LFX_DEVTYPE_DISPLAY

#define API_BUFFLEN 8192

namespace lightfx {
    namespace devices {

        DeviceLightpack::DeviceLightpack(const string& hostname, const string& port, const string& key) {
            this->hostname = hostname;
            this->port = port;
            this->key = key;
        }

        bool DeviceLightpack::Initialize() {
            if (!this->IsInitialized()) {
                if (this->ConnectAPI()) {
                    this->Lights.clear();

                    int amountLeds = this->GetCountLeds();
                    auto leds = this->GetLeds();
                    LightpackScreen screen = this->GetScreenSize();
                    double divider = max(screen.width - screen.x, screen.height - screen.y) / 100.0;
                    for (int i = 0; i < amountLeds; ++i) {
                        DeviceLight light;
                        light.Name = to_wstring(leds[i].index);
                        int posX = int(((leds[i].x - screen.x) + (leds[i].width / 2)) / divider);
                        int posY = int(((screen.height - leds[i].y - screen.y) + (leds[i].height / 2)) / divider);
                        light.Position = { posX, posY, 0 };
                        this->Lights.push_back(light);
                    }

                    return DeviceBase::Initialize();
                }
            }
            return true;
        }

        bool DeviceLightpack::Release() {
            if (this->IsInitialized()) {
                this->DisconnectAPI();
                return DeviceBase::Release();
            }
            return true;
        }

        bool DeviceLightpack::Update() {
            if (!this->IsEnabled()) {
                return true;
            }

            if (!DeviceBase::Update()) {
                return false;
            }

            vector<LightpackColor> newLights = {};
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                double brightness = this->CurrentPrimaryColor[i].brightness / 255.0;
                int red = int(this->CurrentPrimaryColor[i].red * brightness);
                int green = int(this->CurrentPrimaryColor[i].green * brightness);
                int blue = int(this->CurrentPrimaryColor[i].blue * brightness);
                newLights.push_back({ i + 1, red, green, blue });
            }
            this->SetColors(newLights);
            return true;
        }
            
        wstring DeviceLightpack::GetDeviceName() {
            return DEVICENAME;
        }

        unsigned char DeviceLightpack::GetDeviceType() {
            return DEVICETYPE;
        }

        bool DeviceLightpack::ConnectAPI() {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                // WSAStartup failed
                Log("WSAStartup failed");
                return false;
            }

            struct addrinfo *result = NULL, hints;

            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            if (getaddrinfo(this->hostname.c_str(), this->port.c_str(), &hints, &result) != 0) {
                // getaddrinfo failed
                Log("getaddrinfo failed");
                WSACleanup();
                return false;
            }

            this->lightpackSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (this->lightpackSocket == INVALID_SOCKET) {
                // Invalid socket
                Log("socket failed");
                freeaddrinfo(result);
                WSACleanup();
                return false;
            }

            if (connect(this->lightpackSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
                // Connection error
                Log("connect failed");
                closesocket(this->lightpackSocket);
                this->lightpackSocket = INVALID_SOCKET;
                freeaddrinfo(result);
                WSACleanup();
                return false;
            }

            freeaddrinfo(result);
            this->ReceiveAPI();
            if (this->ApiKey(this->key) == Success) {
                if (this->Lock() != Success) {
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

        bool DeviceLightpack::SendAPI(const string& cmd) {
            string s = cmd + "\n";
            int result = send(this->lightpackSocket, s.c_str(), s.length(), 0);
            if (result == SOCKET_ERROR) {
                closesocket(this->lightpackSocket);
                WSACleanup();
                return false;
            }
            return true;
        }

        string DeviceLightpack::ReceiveAPI() {
            char buff[API_BUFFLEN];
            int result = recv(this->lightpackSocket, buff, API_BUFFLEN, 0);
            if (result > 0) {
                string result = string(buff);
                result = result.substr(0, result.find("\n") - 1);
                return result;
            } else if (result == 0) {
                // Connection closed
                return "";
            } else {
                // Error
                return "";
            }
        }

        LightpackStatus DeviceLightpack::ApiKey(const string& key) {
            this->SendAPI("apikey:" + key);
            string result = this->ReceiveAPI();
            if (result == "ok") {
                return Success;
            } else if (result == "fail") {
                return Fail;
            } else {
                return Error;
            }
        }

        LightpackStatus DeviceLightpack::Lock() {
            this->SendAPI("lock");
            string result = this->ReceiveAPI();
            if (result == "lock:success") {
                return Success;
            } else if (result == "lock:busy") {
                return Busy;
            } else {
                return Error;
            }
        }

        LightpackStatus DeviceLightpack::Unlock() {
            this->SendAPI("unlock");
            string result = this->ReceiveAPI();
            if (result == "unlock:success") {
                return Success;
            } else if (result == "unlock:not locked") {
                return NotLocked;
            } else {
                return Error;
            }
        }

        int DeviceLightpack::GetCountLeds() {
            this->SendAPI("getcountleds");
            string result = this->ReceiveAPI();
            size_t pos = result.find(":");
            if (pos != string::npos) {
                result.erase(0, pos + 1);
                return atoi(result.c_str());
            } else {
                return 0;
            }
        }

        vector<LightpackLed> DeviceLightpack::GetLeds() {
            vector<LightpackLed> v;
            this->SendAPI("getleds");
            string result = this->ReceiveAPI();

            // Find delimiter ':' first to seperate the result from the used command
            size_t pos = result.find(":");
            if (pos != string::npos) {
                result.erase(0, pos + 1);
                // Repeat until end of string
                while (pos != string::npos) {
                    // Find delimter '-' to get the LED index
                    pos = result.find("-");
                    if (pos != string::npos) {
                        LightpackLed led;
                        string subresult = result.substr(0, pos);
                        result.erase(0, pos + 1);
                        led.index = atoi(subresult.c_str());
                        // Find 3x delimiter ',' to get the x, y and width of the captured area
                        for (int i = 0; i < 3; ++i) {
                            pos = result.find(",");
                            if (pos != string::npos) {
                                subresult = result.substr(0, pos);
                                result.erase(0, pos + 1);
                                switch (i)
                                {
                                case 0:
                                    led.x = atoi(subresult.c_str());
                                    break;
                                case 1:
                                    led.y = atoi(subresult.c_str());
                                    break;
                                case 2:
                                    led.width = atoi(subresult.c_str());
                                    break;
                                }
                            }
                        }
                        // Find last delimiter ';' to get the height of the captured area
                        pos = result.find(";");
                        if (pos != string::npos) {
                            subresult = result.substr(0, pos);
                            result.erase(0, pos + 1);
                            led.height = atoi(subresult.c_str());
                        }
                        v.push_back(led);
                    }
                }
            }
            return v;
        }

        LightpackScreen DeviceLightpack::GetScreenSize() {
            // Command is not working properly, so determine screensize by the LED positions
            auto leds = this->GetLeds();
            LightpackScreen result = {};
            for (auto led : leds) {
                result.width = max(result.width, led.x + led.width);
                result.height = max(result.height, led.y + led.height);
            }

            return result;

            /*
            LightpackScreen screen = {};
            this->SendAPI("getleds");
            string result = this->ReceiveAPI();

            // Find delimiter ':' first to seperate the result from the used command
            size_t pos = result.find(":");
            if (pos != string::npos) {
                result.erase(0, pos + 1);
                // Find delimter ',' to seperate x, y and width
                for (int i = 0; i < 3; ++i) {
                    pos = result.find(",");
                    if (pos != string::npos) {
                        string subresult = result.substr(0, pos);
                        result.erase(0, pos + 1);
                        switch (i)
                        {
                        case 0:
                            screen.x = atoi(subresult.c_str());
                            break;
                        case 1:
                            screen.y = atoi(subresult.c_str());
                            break;
                        case 2:
                            screen.width = atoi(subresult.c_str());
                            break;
                        }
                    }
                }
                // Last bit is the height
                screen.height = atoi(result.c_str());
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
            string cmd = "setcolor:";
            for (size_t i = 0; i < indices.size(); ++i) {
                cmd += to_string(indices[i]) + "-" + to_string(red[i]) + "," + to_string(green[i]) + "," + to_string(blue[i]) + ";";
            }
            this->SendAPI(cmd);
            string result = this->ReceiveAPI();
            if (result == "ok") {
                return Success;
            } else if (result == "busy") {
                return Busy;
            } else if (result == "not locked") {
                return NotLocked;
            } else {
                return Error;
            }
        }

        LightpackStatus DeviceLightpack::SetColors(const vector<LightpackColor>& colors) {
            string cmd = "setcolor:";
            for (size_t i = 0; i < colors.size(); ++i) {
                cmd += to_string(colors[i].index) + "-" + to_string(colors[i].red) + "," + to_string(colors[i].green) + "," + to_string(colors[i].blue) + ";";
            }
            this->SendAPI(cmd);
            string result = this->ReceiveAPI();
            if (result == "ok") {
                return Success;
            } else if (result == "busy") {
                return Busy;
            } else if (result == "not locked") {
                return NotLocked;
            } else {
                return Error;
            }
        }

    }
}

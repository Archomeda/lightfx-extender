#pragma once
#include "DeviceBase.h"

#include <WinSock2.h>

namespace lightfx {
    namespace devices {

        enum LightpackStatus {
            Success = 0,
            Fail,
            On,
            Off,
            Busy,
            Idle,
            NotLocked,
            Error,
            Unknown
        };

        struct LightpackLed {
            int index;
            int x;
            int y;
            int width;
            int height;
        };

        struct LightpackColor {
            int index;
            int red;
            int green;
            int blue;
        };

        struct LightpackScreen{
            int x;
            int y;
            int width;
            int height;
        };

        class DeviceLightpack : public DeviceBase {

        public:
            DeviceLightpack(const std::string& hostname, const std::string& port, const std::string& key);

            virtual LFX_RESULT Initialize() override;
            virtual LFX_RESULT Release() override;
            virtual LFX_RESULT Update() override;

            virtual LFX_RESULT GetDeviceInfo(std::string& description, unsigned char& type) override;

        protected:
            bool ConnectAPI();
            bool DisconnectAPI();
            bool SendAPI(const std::string& cmd);
            std::string ReceiveAPI();

            LightpackStatus ApiKey(const std::string& key);
            LightpackStatus Lock();
            LightpackStatus Unlock();
            int GetCountLeds();
            std::vector<LightpackLed> GetLeds();
            LightpackScreen GetScreenSize();
            LightpackStatus SetColor(const int index, const int red, const int green, const int blue);
            LightpackStatus SetColor(const LightpackColor& color);
            LightpackStatus SetColors(const std::vector<int>& indices, const std::vector<int>& red, const std::vector<int>& green, const std::vector<int>& blue);
            LightpackStatus SetColors(const std::vector<LightpackColor>& colors);

        private:
            SOCKET lightpackSocket = INVALID_SOCKET;
            std::string hostname = "127.0.0.1";
            std::string port = "3636";
            std::string key = "";

        };

    }
}

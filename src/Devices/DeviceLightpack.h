#pragma once

// Windows includes
#include "../Common/Windows.h" // Since WinSock2.h includes Windows.h we need to make sure to configure some things first
#include <WinSock2.h>

// Project includes
#include "DeviceBase.h"


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
            DeviceLightpack(const std::wstring& hostname, const std::wstring& port, const std::wstring& key);

            virtual bool Initialize() override;
            virtual bool Release() override;

            virtual bool PushColorToDevice() override;

            virtual std::wstring GetDeviceName() override;
            virtual unsigned char GetDeviceType() override;

        protected:
            bool ConnectAPI();
            bool DisconnectAPI();
            bool SendAPI(const std::wstring& cmd);
            std::wstring ReceiveAPI();

            LightpackStatus ApiKey(const std::wstring& key);
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
            std::wstring hostname = L"127.0.0.1";
            std::wstring port = L"3636";
            std::wstring key = L"";

        };

    }
}

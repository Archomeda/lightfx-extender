#pragma once

// Windows includes
#include "../Common/Windows.h"
#include <WinSock2.h>

// Project includes
#include "Device.h"

namespace lightfx {
    namespace devices {

        enum LightpackStatus {
            LightpackSuccess = 0,
            LightpackFail,
            LightpackOn,
            LightpackOff,
            LightpackBusy,
            LightpackIdle,
            LightpackNotLocked,
            LightpackError,
            LightpackUnknown
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

        class DeviceLightpack : public Device {

        public:
            DeviceLightpack(const std::wstring& hostname, const std::wstring& port, const std::wstring& key);

            virtual bool Initialize() override;
            virtual bool Enable() override;
            virtual bool Disable() override;

            virtual const std::wstring GetDeviceName() override { return L"Lightpack"; }
            virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceDisplay; }

        protected:
            virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

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

#pragma once

// Standard includes
#include <string>
#include <vector>

// Windows includes
#include "../../Common/Windows.h"
#include <WinSock2.h>

// Project includes
#include "LibraryBase.h"

// API exports
#include "../../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace vendors {
        namespace libraries {

            enum LightpackStatus {
                LightpackSuccess,
                LightpackFail,
                LightpackBusy,
                LightpackIdle,
                LightpackNotLocked,
                LightpackDeviceError,
                LightpackOn,
                LightpackOff,
                LightpackError,
                LightpackUnknown,
                LightpackUnknownCommand
            };

            struct LightpackLed {
                size_t index;
                int x;
                int y;
                int width;
                int height;
            };

            struct LightpackColor {
                size_t index;
                int red;
                int green;
                int blue;
            };

            struct LightpackScreen {
                int x;
                int y;
                int width;
                int height;
            };

            class LFXE_API LibraryLightpack : public LibraryBase {

            public:
                LibraryLightpack() {}
                virtual ~LibraryLightpack();

                void SetHostname(const std::wstring& hostname) {
                    this->hostname = hostname;
                }
                std::wstring GetHostname() const {
                    return this->hostname;
                }
                void SetPort(const std::wstring& port) {
                    this->port = port;
                }
                std::wstring GetPort() const {
                    return this->port;
                }

                virtual bool IsLibraryAvailable() const override;
                virtual bool InitializeLibrary() override;
                virtual bool ReleaseLibrary() override;

                virtual lightfx::Version GetLibraryVersion() const override;

                // Library functions
                LightpackStatus Lp_Authorize(const std::wstring& key) const;
                LightpackStatus Lp_Lock() const;
                LightpackStatus Lp_Unlock() const;
                LightpackStatus Lp_GetStatus() const;
                LightpackStatus Lp_GetStatusApi() const;
                std::wstring Lp_GetCurrentProfile() const;
                std::vector<std::wstring> Lp_GetProfiles() const;
                int Lp_GetCountLeds() const;
                std::vector<LightpackLed> Lp_GetLeds() const;
                std::vector<LightpackColor> Lp_GetColors() const;
                double Lp_GetFps() const;
                LightpackScreen Lp_GetScreenSize() const;
                std::wstring Lp_GetMode() const;
                LightpackStatus Lp_SetColors(const std::vector<LightpackColor>& colors) const;
                LightpackStatus Lp_SetLeds(const std::vector<LightpackLed>& leds) const;
                LightpackStatus Lp_SetGamma(const double gamma) const;
                LightpackStatus Lp_SetBrightness(const int brightness) const;
                LightpackStatus Lp_SetSmooth(const int smooth) const;
                LightpackStatus Lp_SetCurrentProfile(const std::wstring& profile) const;
                LightpackStatus Lp_NewProfile(const std::wstring& profile) const;
                LightpackStatus Lp_DeleteProfile(const std::wstring& profile) const;
                LightpackStatus Lp_SetStatus(bool active) const;
                LightpackStatus Lp_SetMode(const std::wstring& mode) const;

                std::wstring LightpackStatusToString(const LightpackStatus status) const;

            protected:
                bool Connect() const;
                bool Disconnect() const;
                bool Send(const std::wstring& command) const;
                std::wstring Receive() const;
                std::wstring StripLeadingResult(const std::wstring& str) const;
                std::vector<std::wstring> SplitList(const std::wstring& str, const char delim = ';') const;
                LightpackStatus ParseStatus(const std::wstring& str) const;

                mutable Version apiVersion;

            private:
                mutable SOCKET lightpackSocket = INVALID_SOCKET;
                std::wstring hostname = L"127.0.0.1";
                std::wstring port = L"3636";

            };

        }
    }
}

#pragma warning(pop)

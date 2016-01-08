#pragma once

// Standard includes
#include <string>

// Windows includes
#include "../../Common/Windows.h"
#include <Windows.h>

// 3rd party includes
#include "LFX2.h"

// Project includes
#include "LibraryBase.h"

// API exports
#include "../../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace vendors {
        namespace libraries {

            enum AlienwareResult : unsigned int {
                AlienwareSuccess = 0,
                AlienwareFailure,
                AlienwareErrorNoInit,
                AlienwareErrorNoDevices,
                AlienwareErrorNoLights,
                AlienwareErrorBufferSizeTooSmall
            };

            enum AlienwareDeviceType : unsigned int {
                AlienwareDeviceTypeUnknown = 0x00,
                AlienwareDeviceTypeNotebook,
                AlienwareDeviceTypeDesktop,
                AlienwareDeviceTypeServer,
                AlienwareDeviceTypeDisplay,
                AlienwareDeviceTypeMouse,
                AlienwareDeviceTypeKeyboard,
                AlienwareDeviceTypeGamepad,
                AlienwareDeviceTypeSpeaker,
                AlienwareDeviceTypeOther = 0xFF
            };

            enum AlienwareActionType : unsigned int {
                AlienwareActionTypeMorph = 0x01,
                AlienwareActionTypePulse,
                AlienwareActionTypeColor
            };

            class LFXE_API LibraryAlienware : public LibraryBase {

            public:
                LibraryAlienware() {}
                virtual ~LibraryAlienware();

                void SetBackupFilename(const std::wstring& filename) {
                    this->backupFilename = filename;
                }
                std::wstring GetBackupFilename() const {
                    return this->backupFilename;
                }
                
                virtual bool IsLibraryAvailable() const override;
                virtual bool InitializeLibrary() override;
                virtual bool ReleaseLibrary() override;

                virtual lightfx::Version GetLibraryVersion() const override;

                // Library functions
                AlienwareResult Aw_Initialize() const;
                AlienwareResult Aw_Release() const;
                AlienwareResult Aw_Reset() const;
                AlienwareResult Aw_Update() const;
                AlienwareResult Aw_UpdateDefault() const;
                AlienwareResult Aw_GetNumDevices(unsigned int* const numDevices) const;
                AlienwareResult Aw_GetDeviceDescription(const unsigned int devIndex, std::wstring* const devDesc, AlienwareDeviceType* const devType) const;
                AlienwareResult Aw_GetNumLights(const unsigned int devIndex, unsigned int* const numLights) const;
                AlienwareResult Aw_GetLightDescription(const unsigned int devIndex, const unsigned int lightIndex, std::wstring* const lightDesc) const;
                AlienwareResult Aw_GetLightLocation(const unsigned int devIndex, const unsigned int lightIndex, PLFX_POSITION const lightLoc) const;
                AlienwareResult Aw_GetLightColor(const unsigned int devIndex, const unsigned int lightIndex, PLFX_COLOR const lightCol) const;
                AlienwareResult Aw_SetLightColor(const unsigned int devIndex, const unsigned int lightIndex, const LFX_COLOR& lightCol) const;
                AlienwareResult Aw_Light(const unsigned int locationMask, const LFX_COLOR& lightCol) const;
                AlienwareResult Aw_SetLightActionColor(const unsigned int devIndex, const unsigned int lightIndex, const AlienwareActionType actionType, const LFX_COLOR& primaryCol) const;
                AlienwareResult Aw_SetLightActionColorEx(const unsigned int devIndex, const unsigned int lightIndex, const AlienwareActionType actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) const;
                AlienwareResult Aw_ActionColor(const unsigned int locationMask, const AlienwareActionType actionType, const LFX_COLOR& primaryCol) const;
                AlienwareResult Aw_ActionColorEx(const unsigned int locationMask, const AlienwareActionType actionType, const LFX_COLOR& primaryCol, const LFX_COLOR& secondaryCol) const;
                AlienwareResult Aw_SetTiming(const int newTiming) const;
                AlienwareResult Aw_GetVersion(std::wstring* const version) const;

                LFX_COLOR UIntToLfxColor(const unsigned int color) const;
                unsigned int LfxColorToUInt(const LFX_COLOR color) const;
                std::wstring AlienwareResultToString(const AlienwareResult result) const;

            protected:
                bool LoadDll() const;
                bool LoadExports();
                bool UnloadDll() const;
                bool UnloadExports();

            private:
                mutable HINSTANCE hInstance = NULL;
                
                std::wstring backupFilename = L"LightFX_.dll";

                LFX2INITIALIZE LFX_Initialize;
                LFX2RELEASE LFX_Release;
                LFX2RESET LFX_Reset;
                LFX2UPDATE LFX_Update;
                LFX2UPDATEDEFAULT LFX_UpdateDefault;
                LFX2GETNUMDEVICES LFX_GetNumDevices;
                LFX2GETDEVDESC LFX_GetDeviceDescription;
                LFX2GETNUMLIGHTS LFX_GetNumLights;
                LFX2GETLIGHTDESC LFX_GetLightDescription;
                LFX2GETLIGHTLOC LFX_GetLightLocation;
                LFX2GETLIGHTCOL LFX_GetLightColor;
                LFX2SETLIGHTCOL LFX_SetLightColor;
                LFX2LIGHT LFX_Light;
                LFX2SETLIGHTACTIONCOLOR LFX_SetLightActionColor;
                LFX2SETLIGHTACTIONCOLOREX LFX_SetLightActionColorEx;
                LFX2ACTIONCOLOR LFX_ActionColor;
                LFX2ACTIONCOLOREX LFX_ActionColorEx;
                LFX2SETTIMING LFX_SetTiming;
                LFX2GETVERSION LFX_GetVersion;

            };

        }
    }
}

#pragma warning(pop)

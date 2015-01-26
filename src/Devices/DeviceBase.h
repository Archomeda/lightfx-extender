#pragma once

// Standard includes
#include <string>
#include <vector>

// 3rd party includes
#include "LFXDecl.h"

// Project includes
#include "../LightLocationMask.h"
#include "DeviceLight.h"


namespace lightfx {
    namespace devices {

        enum LightAction {
            Instant,
            Morph,
        };

        class DeviceBase {

        public:
            DeviceBase();

            bool IsEnabled() { return this->IsEnabled_; }
            bool IsInitialized() { return this->IsInitialized_; }

            virtual bool EnableDevice();
            virtual bool DisableDevice();

            virtual bool Initialize();
            virtual bool Release();
            virtual bool Update();
            virtual bool Reset();

            virtual bool PushColorToDevice() = 0;

            virtual std::wstring GetDeviceName() = 0;
            virtual unsigned char GetDeviceType() = 0;

            virtual size_t GetNumberOfLights();
            virtual DeviceLight GetLight(const size_t index);

            virtual LFX_COLOR GetColorForLight(const size_t index);
            virtual bool SetColor(const LFX_COLOR& color);
            virtual bool SetColorForLight(const size_t index, const LFX_COLOR& color);
            virtual bool SetColorForLocation(const LightLocationMask locationMask, const LFX_COLOR& color);

            virtual bool MorphTo(const LFX_COLOR& color, unsigned int time);
            virtual bool MorphToForLight(const size_t index, const LFX_COLOR& color, unsigned int time);
            virtual bool MorphToForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, unsigned int time);

        protected:
            bool AnimationRunning = false;
            unsigned long AnimationStartTime = 0;
            virtual void AnimateCurrentColorLoop();

            bool IsEnabled_ = false;
            bool IsInitialized_ = false;

            std::vector<DeviceLight> Lights = {};
            std::vector<LFX_COLOR> CurrentColor = {};

            LightAction CurrentAction = Instant;
            std::vector<LFX_COLOR> CurrentActionStartColor = {};
            std::vector<LFX_COLOR> CurrentActionEndColor = {};
            unsigned int CurrentActionTime = 200;

            LightAction NextAction = Instant;
            std::vector<LFX_COLOR> NextActionStartColor = {};
            std::vector<LFX_COLOR> NextActionEndColor = {};
            unsigned int NextActionTime = 200;
        };

    }
}

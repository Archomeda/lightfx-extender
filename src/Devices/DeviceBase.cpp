#include "DeviceBase.h"

// Standard includes
#include <chrono>

// Windows includes
#include "../Common/Windows.h"
#include <Windows.h>

// Project includes
#include "../Utils/Log.h"


using namespace std;
using namespace lightfx::utils;

namespace lightfx {
    namespace devices {

        DeviceBase::DeviceBase() {
            this->Reset();
        }

        DeviceBase::~DeviceBase() {
            this->StopAnimating();
        }

        bool DeviceBase::EnableDevice() {
            if (!this->IsEnabled()) {
                if (this->Initialize()) {
                    Log(this->GetDeviceName() + L" enabled");
                    this->IsEnabled_ = true;
                    return true;
                }
                return false;
            }
            return true;
        }

        bool DeviceBase::DisableDevice() {
            if (this->IsEnabled()) {
                if (this->Release()) {
                    Log(this->GetDeviceName() + L" disabled");
                    this->IsEnabled_ = false;
                    return true;
                }
                return false;
            }
            return true;
        }


        bool DeviceBase::Initialize() {
            if (!this->IsInitialized()) {
                this->IsInitialized_ = true;
                this->Reset();
            }
            return true;
        }

        bool DeviceBase::Release() {
            if (this->IsInitialized()) {
                this->IsInitialized_ = false;
            }
            return true;
        }

        bool DeviceBase::Update() {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->CurrentActionStartColor[i] = this->NextActionStartColor[i];
                this->CurrentActionEndColor[i] = this->NextActionEndColor[i];
            }
            this->CurrentAction = this->NextAction;
            this->CurrentActionTime = this->NextActionTime;
            this->CurrentActionAmount = this->NextActionAmount;

            if (this->CurrentAction == LightAction::Instant) {
                for (size_t i = 0; i < this->Lights.size(); ++i) {
                    this->CurrentColor[i] = this->NextActionEndColor[i];
                }
                return this->PushColorToDevice();
            } else {
                this->animationStartTime = GetTickCount();
                this->StopAnimating();
                this->StartAnimating();
            }

            return true;
        }

        bool DeviceBase::Reset() {
            this->CurrentColor = vector<LFX_COLOR>(this->Lights.size());
            this->CurrentAction = LightAction::Instant;
            this->CurrentActionStartColor = vector<LFX_COLOR>(this->Lights.size());
            this->CurrentActionEndColor = vector<LFX_COLOR>(this->Lights.size());
            this->CurrentActionTime = 200;
            this->CurrentActionAmount = 5;
            this->NextAction = LightAction::Instant;
            this->NextActionStartColor = vector<LFX_COLOR>(this->Lights.size());
            this->NextActionEndColor = vector<LFX_COLOR>(this->Lights.size());
            this->NextActionTime = 200;
            this->NextActionAmount = 5;
            return true;
        }


        size_t DeviceBase::GetNumberOfLights() {
            return this->Lights.size();
        }

        DeviceLight DeviceBase::GetLight(const size_t index) {
            if (index < this->Lights.size()) {
                return this->Lights[index];
            }
            return DeviceLight();
        }

        LFX_COLOR DeviceBase::GetColorForLight(const size_t index) {
            if (index < this->Lights.size()) {
                return LFX_COLOR(this->CurrentColor[index]);
            }
            return LFX_COLOR();
        }

        bool DeviceBase::SetColor(const LFX_COLOR& color) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextActionEndColor[i] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Instant;
            return true;
        }

        bool DeviceBase::SetColorForLight(const size_t index, const LFX_COLOR& color) {
            if (index < this->Lights.size()) {
                this->NextActionEndColor[index] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Instant;
            return true;
        }

        bool DeviceBase::SetColorForLocation(const LightLocationMask locationMask, const LFX_COLOR& color) {
            // TODO: Implement locationMask filter
            return this->SetColor(color);
        }

        bool DeviceBase::MorphTo(const LFX_COLOR& color, unsigned int time) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextActionStartColor[i] = LFX_COLOR(this->CurrentActionEndColor[i]);
                this->NextActionEndColor[i] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Morph;
            this->NextActionTime = time > 0 ? time : 200;
            return true;
        }

        bool DeviceBase::MorphToForLight(const size_t index, const LFX_COLOR& color, unsigned int time) {
            if (index < this->Lights.size()) {
                this->NextActionStartColor[index] = LFX_COLOR(this->CurrentActionEndColor[index]);
                this->NextActionEndColor[index] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Morph;
            this->NextActionTime = time > 0 ? time : 200;
            return true;
        }

        bool DeviceBase::MorphToForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, unsigned int time) {
            // TODO: Implement locationMask filter
            return this->MorphTo(color, time);
        }

        bool DeviceBase::Pulse(const LFX_COLOR& color, unsigned int time, unsigned int amount) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextActionStartColor[i] = LFX_COLOR(this->CurrentActionEndColor[i]);
                this->NextActionEndColor[i] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Pulse;
            this->NextActionTime = time > 0 ? time : 200;
            this->NextActionAmount = amount > 0 ? amount : 5;
            return true;
        }

        bool DeviceBase::PulseForLight(const size_t index, const LFX_COLOR& color, unsigned int time, unsigned int amount) {
            if (index < this->Lights.size()) {
                this->NextActionStartColor[index] = LFX_COLOR(this->CurrentActionEndColor[index]);
                this->NextActionEndColor[index] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Pulse;
            this->NextActionTime = time > 0 ? time : 200;
            this->NextActionAmount = amount > 0 ? amount : 5;
            return true;
        }

        bool DeviceBase::PulseForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, unsigned int time, unsigned int amount) {
            // TODO: Implement locationMask filter
            return this->Pulse(color, time, amount);
        }

        void DeviceBase::AnimateCurrentColorLoop() {
            while (this->animationRunning) {
                unsigned long timePassed = GetTickCount() - this->animationStartTime;
                double progress = (double)timePassed / this->CurrentActionTime;
                bool colorChanged = false;

                switch (this->CurrentAction) {
                case LightAction::Morph:
                    for (size_t i = 0; i < this->Lights.size(); ++i) {
                        unsigned char newR = this->CurrentActionStartColor[i].red + unsigned char(progress * (this->CurrentActionEndColor[i].red - this->CurrentActionStartColor[i].red));
                        unsigned char newG = this->CurrentActionStartColor[i].green + unsigned char(progress * (this->CurrentActionEndColor[i].green - this->CurrentActionStartColor[i].green));
                        unsigned char newB = this->CurrentActionStartColor[i].blue + unsigned char(progress * (this->CurrentActionEndColor[i].blue - this->CurrentActionStartColor[i].blue));
                        unsigned char newBr = this->CurrentActionStartColor[i].brightness + unsigned char(progress * (this->CurrentActionEndColor[i].brightness - this->CurrentActionStartColor[i].brightness));

                        if (newR != this->CurrentColor[i].red || newG != this->CurrentColor[i].green || newB != this->CurrentColor[i].blue || newBr != this->CurrentColor[i].brightness) {
                            this->CurrentColor[i].red = newR;
                            this->CurrentColor[i].green = newG;
                            this->CurrentColor[i].blue = newB;
                            this->CurrentColor[i].brightness = newBr;
                            colorChanged = true;
                        }
                    }

                    if (colorChanged) {
                        this->PushColorToDevice();
                    }

                    if (timePassed >= this->CurrentActionTime) {
                        this->animationRunning = false;
                        return;
                    }

                    break;

                case LightAction::Pulse:
                    double intpart;
                    progress = modf(progress, &intpart);
                    for (size_t i = 0; i < this->Lights.size(); ++i) {
                        unsigned char newR, newG, newB, newBr;
                        if (progress < 0.5) {
                            newR = this->CurrentActionStartColor[i].red + unsigned char(progress * 2 * (this->CurrentActionEndColor[i].red - this->CurrentActionStartColor[i].red));
                            newG = this->CurrentActionStartColor[i].green + unsigned char(progress * 2 * (this->CurrentActionEndColor[i].green - this->CurrentActionStartColor[i].green));
                            newB = this->CurrentActionStartColor[i].blue + unsigned char(progress * 2 * (this->CurrentActionEndColor[i].blue - this->CurrentActionStartColor[i].blue));
                            newBr = this->CurrentActionStartColor[i].brightness + unsigned char(progress * 2 * (this->CurrentActionEndColor[i].brightness - this->CurrentActionStartColor[i].brightness));
                        } else {
                            newR = this->CurrentActionEndColor[i].red - unsigned char((progress - 0.5) * 2 * (this->CurrentActionEndColor[i].red - this->CurrentActionStartColor[i].red));
                            newG = this->CurrentActionEndColor[i].green - unsigned char((progress - 0.5) * 2 * (this->CurrentActionEndColor[i].green - this->CurrentActionStartColor[i].green));
                            newB = this->CurrentActionEndColor[i].blue - unsigned char((progress - 0.5) * 2 * (this->CurrentActionEndColor[i].blue - this->CurrentActionStartColor[i].blue));
                            newBr = this->CurrentActionEndColor[i].brightness - unsigned char((progress - 0.5) * 2 * (this->CurrentActionEndColor[i].brightness - this->CurrentActionStartColor[i].brightness));
                        }

                        if (newR != this->CurrentColor[i].red || newG != this->CurrentColor[i].green || newB != this->CurrentColor[i].blue || newBr != this->CurrentColor[i].brightness) {
                            this->CurrentColor[i].red = newR;
                            this->CurrentColor[i].green = newG;
                            this->CurrentColor[i].blue = newB;
                            this->CurrentColor[i].brightness = newBr;
                            colorChanged = true;
                        }
                    }

                    if (colorChanged) {
                        this->PushColorToDevice();
                    }

                    if (timePassed >= this->CurrentActionTime * this->CurrentActionAmount) {
                        this->animationRunning = false;
                        return;
                    }

                    break;

                default:
                    this->animationRunning = false;
                    return;
                }

                this_thread::sleep_for(chrono::milliseconds(1));
            }
        }

        void DeviceBase::StartAnimating() {
            this->animationRunning = true;
            this->hasAnimated = true;
            this->animationThread = thread(&DeviceBase::AnimateCurrentColorLoop, this);
        }

        void DeviceBase::StopAnimating() {
            if (this->animationRunning || this->hasAnimated) {
                this->animationRunning = false;
                this->hasAnimated = false;
                this->animationThread.join();
                this->animationThread.~thread();
            }
        }
    }
}


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
            this->CurrentActionTransitionTime = this->NextActionTransitionTime;
            this->CurrentActionStartColorTime = this->NextActionStartColorTime;
            this->CurrentActionEndColorTime = this->NextActionEndColorTime;
            this->CurrentActionAmount = this->NextActionAmount;

            this->StopAnimating();
            if (this->CurrentAction == LightAction::Instant) {
                for (size_t i = 0; i < this->Lights.size(); ++i) {
                    this->CurrentColor[i] = this->NextActionEndColor[i];
                }
                return this->PushColorToDevice();
            } else {
                this->animationStartTime = GetTickCount();
                this->StartAnimating();
            }

            return true;
        }

        bool DeviceBase::Reset() {
            this->CurrentColor = vector<LFX_COLOR>(this->Lights.size());
            this->CurrentAction = LightAction::Instant;
            this->CurrentActionStartColor = vector<LFX_COLOR>(this->Lights.size());
            this->CurrentActionEndColor = vector<LFX_COLOR>(this->Lights.size());
            this->CurrentActionTransitionTime = 200;
            this->CurrentActionStartColorTime = 0;
            this->CurrentActionEndColorTime = 0;
            this->CurrentActionAmount = 5;
            this->NextAction = LightAction::Instant;
            this->NextActionStartColor = vector<LFX_COLOR>(this->Lights.size());
            this->NextActionEndColor = vector<LFX_COLOR>(this->Lights.size());
            this->NextActionTransitionTime = 200;
            this->NextActionStartColorTime = 0;
            this->NextActionEndColorTime = 0;
            this->NextActionAmount = 5;
            this->PrevStaticColor = vector<LFX_COLOR>(this->Lights.size());
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

        bool DeviceBase::MorphTo(const LFX_COLOR& color, const unsigned int transitionTime) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                this->NextActionStartColor[i] = LFX_COLOR(this->CurrentActionEndColor[i]);
                this->NextActionEndColor[i] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Morph;
            this->NextActionTransitionTime = transitionTime > 0 ? transitionTime : 200;
            return true;
        }

        bool DeviceBase::MorphToForLight(const size_t index, const LFX_COLOR& color, const unsigned int transitionTime) {
            if (index < this->Lights.size()) {
                this->NextActionStartColor[index] = LFX_COLOR(this->CurrentActionEndColor[index]);
                this->NextActionEndColor[index] = LFX_COLOR(color);
            }
            this->NextAction = LightAction::Morph;
            this->NextActionTransitionTime = transitionTime > 0 ? transitionTime : 200;
            return true;
        }

        bool DeviceBase::MorphToForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, const unsigned int transitionTime) {
            // TODO: Implement locationMask filter
            return this->MorphTo(color, transitionTime);
        }

        bool DeviceBase::Pulse(const LFX_COLOR& color, const unsigned int transitionTime, const unsigned int amount) {
            return this->Pulse(color, transitionTime, 0, 0, amount);
        }

        bool DeviceBase::Pulse(const LFX_COLOR& color, const unsigned int transitionTime, const unsigned int startColorTime, const unsigned int endColorTime, const unsigned int amount) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                if (!this->PulseForLight(i, color, transitionTime, startColorTime, endColorTime, amount)) {
                    return false;
                }
            }
            return true;
        }

        bool DeviceBase::Pulse(const LFX_COLOR& startColor, const LFX_COLOR& endColor, const unsigned int transitionTime, const unsigned int amount) {
            return this->Pulse(startColor, endColor, transitionTime, 0, 0, amount);
        }

        bool DeviceBase::Pulse(const LFX_COLOR& startColor, const LFX_COLOR& endColor, const unsigned int transitionTime, const unsigned int startColorTime, const unsigned int endColorTime, const unsigned int amount) {
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                if (!this->PulseForLight(i, startColor, endColor, transitionTime, startColorTime, endColorTime, amount)) {
                    return false;
                }
            }
            return true;
        }

        bool DeviceBase::PulseForLight(const size_t index, const LFX_COLOR& color, const unsigned int transitionTime, const unsigned int amount) {
            return this->PulseForLight(index, color, transitionTime, 0, 0, amount);
        }

        bool DeviceBase::PulseForLight(const size_t index, const LFX_COLOR& color, const unsigned int transitionTime, const unsigned int startColorTime, const unsigned int endColorTime, const unsigned int amount) {
            return this->PulseForLight(index, this->CurrentActionEndColor[index], color, transitionTime, startColorTime, endColorTime, amount);
        }

        bool DeviceBase::PulseForLight(const size_t index, const LFX_COLOR& startColor, const LFX_COLOR& endColor, const unsigned int transitionTime, const unsigned int amount) {
            return this->PulseForLight(index, startColor, endColor, transitionTime, 0, 0, amount);
        }

        bool DeviceBase::PulseForLight(const size_t index, const LFX_COLOR& startColor, const LFX_COLOR& endColor, const unsigned int transitionTime, const unsigned int startColorTime, const unsigned int endColorTime, const unsigned int amount) {
            if (index < this->Lights.size()) {
                this->PrevStaticColor[index] = LFX_COLOR(this->CurrentActionEndColor[index]);
                this->NextActionStartColor[index] = LFX_COLOR(startColor);
                this->NextActionEndColor[index] = LFX_COLOR(endColor);
            }
            this->NextAction = LightAction::Pulse;
            this->NextActionTransitionTime = transitionTime > 0 ? transitionTime : 200;
            this->NextActionStartColorTime = startColorTime;
            this->NextActionEndColorTime = endColorTime;
            this->NextActionAmount = amount > 0 ? amount : 5;
            return true;
        }

        bool DeviceBase::PulseForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, const unsigned int transitionTime, const unsigned int amount) {
            return this->PulseForLocation(locationMask, color, transitionTime, 0, 0, amount);
        }

        bool DeviceBase::PulseForLocation(const LightLocationMask locationMask, const LFX_COLOR& color, const unsigned int transitionTime, const unsigned int startColorTime, const unsigned int endColorTime, const unsigned int amount) {
            // TODO: Implement locationMask filter
            return this->Pulse(color, transitionTime, startColorTime, endColorTime, amount);
        }

        bool DeviceBase::PulseForLocation(const LightLocationMask locationMask, const LFX_COLOR& startColor, const LFX_COLOR& endColor, const unsigned int transitionTime, const unsigned int amount) {
            return this->PulseForLocation(locationMask, startColor, endColor, transitionTime, 0, 0, amount);
        }

        bool DeviceBase::PulseForLocation(const LightLocationMask locationMask, const LFX_COLOR& startColor, const LFX_COLOR& endColor, const unsigned int transitionTime, const unsigned int startColorTime, const unsigned int endColorTime, const unsigned int amount) {
            // TODO: Implement locationMask filter
            return this->Pulse(startColor, endColor, transitionTime, startColorTime, endColorTime, amount);
        }

        void DeviceBase::AnimateCurrentColorLoop() {
            while (this->animationRunning) {
                unsigned long timePassed = GetTickCount() - this->animationStartTime;
                double progress = (double)timePassed / this->CurrentActionTransitionTime;
                bool colorChanged = false;

                switch (this->CurrentAction) {
                case LightAction::Morph:
                    colorChanged = DoAnimateMorph(timePassed);
                    break;

                case LightAction::Pulse:
                    colorChanged = DoAnimatePulse(timePassed);
                    break;

                default:
                    this->animationRunning = false;
                    return;
                }

                if (colorChanged) {
                    this->PushColorToDevice();
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

        bool DeviceBase::DoAnimateMorph(const unsigned long timePassed) {
            bool colorChanged = false;
            double progress = (double)timePassed / this->CurrentActionTransitionTime;

            for (size_t i = 0; i < this->Lights.size(); ++i) {
                unsigned char newR, newG, newB, newBr;
                if (timePassed >= this->CurrentActionTransitionTime) {
                    newR = this->PrevStaticColor[i].red;
                    newG = this->PrevStaticColor[i].green;
                    newB = this->PrevStaticColor[i].blue;
                    newBr = this->PrevStaticColor[i].brightness;
                } else {
                    newR = this->CurrentActionStartColor[i].red + unsigned char(progress * (this->CurrentActionEndColor[i].red - this->CurrentActionStartColor[i].red));
                    newG = this->CurrentActionStartColor[i].green + unsigned char(progress * (this->CurrentActionEndColor[i].green - this->CurrentActionStartColor[i].green));
                    newB = this->CurrentActionStartColor[i].blue + unsigned char(progress * (this->CurrentActionEndColor[i].blue - this->CurrentActionStartColor[i].blue));
                    newBr = this->CurrentActionStartColor[i].brightness + unsigned char(progress * (this->CurrentActionEndColor[i].brightness - this->CurrentActionStartColor[i].brightness));
                }

                if (newR != this->CurrentColor[i].red || newG != this->CurrentColor[i].green || newB != this->CurrentColor[i].blue || newBr != this->CurrentColor[i].brightness) {
                    this->CurrentColor[i].red = newR;
                    this->CurrentColor[i].green = newG;
                    this->CurrentColor[i].blue = newB;
                    this->CurrentColor[i].brightness = newBr;
                    colorChanged = true;
                }
            }

            if (timePassed >= this->CurrentActionTransitionTime) {
                this->animationRunning = false;
            }
            return colorChanged;
        }

        bool DeviceBase::DoAnimatePulse(const unsigned long timePassed) {
            bool colorChanged = false;
            unsigned int timePhase = 2 * this->CurrentActionTransitionTime + this->CurrentActionStartColorTime + this->CurrentActionEndColorTime;
            unsigned int timeTotal = timePhase * this->CurrentActionAmount;
            unsigned int progress = timePassed % timePhase;

            unsigned int progressPhaseStartColor1 = 0;
            unsigned int progressPhaseTransitionToEnd = this->CurrentActionStartColorTime / 2;
            unsigned int progressPhaseEndColor = progressPhaseTransitionToEnd + this->CurrentActionTransitionTime;
            unsigned int progressPhaseTransitionToStart = progressPhaseEndColor + this->CurrentActionEndColorTime;
            unsigned int progressPhaseStartColor2 = progressPhaseTransitionToStart + this->CurrentActionTransitionTime;
            unsigned int progressPhaseEnd = timePhase;
                       
            for (size_t i = 0; i < this->Lights.size(); ++i) {
                unsigned char newR, newG, newB, newBr;
                
                if (timePassed >= timeTotal) {
                    // Time exceeded animation time, reset to previous color
                    newR = this->PrevStaticColor[i].red;
                    newG = this->PrevStaticColor[i].green;
                    newB = this->PrevStaticColor[i].blue;
                    newBr = this->PrevStaticColor[i].brightness;
                } else {
                    if (progress < progressPhaseTransitionToEnd) {
                        // Phase start color 1
                        newR = this->CurrentActionStartColor[i].red;
                        newG = this->CurrentActionStartColor[i].green;
                        newB = this->CurrentActionStartColor[i].blue;
                        newBr = this->CurrentActionStartColor[i].brightness;
                    } else if (progress < progressPhaseEndColor) {
                        // Phase transition to end color
                        double transitionProgress = double(progress - progressPhaseTransitionToEnd) / (progressPhaseEndColor - progressPhaseTransitionToEnd);
                        newR = this->CurrentActionStartColor[i].red + unsigned char(transitionProgress * (this->CurrentActionEndColor[i].red - this->CurrentActionStartColor[i].red));
                        newG = this->CurrentActionStartColor[i].green + unsigned char(transitionProgress * (this->CurrentActionEndColor[i].green - this->CurrentActionStartColor[i].green));
                        newB = this->CurrentActionStartColor[i].blue + unsigned char(transitionProgress * (this->CurrentActionEndColor[i].blue - this->CurrentActionStartColor[i].blue));
                        newBr = this->CurrentActionStartColor[i].brightness + unsigned char(transitionProgress * (this->CurrentActionEndColor[i].brightness - this->CurrentActionStartColor[i].brightness));
                    } else if (progress < progressPhaseTransitionToStart) {
                        // Phase end color
                        newR = this->CurrentActionEndColor[i].red;
                        newG = this->CurrentActionEndColor[i].green;
                        newB = this->CurrentActionEndColor[i].blue;
                        newBr = this->CurrentActionEndColor[i].brightness;
                    } else if (progress < progressPhaseStartColor2) {
                        // Phase transition to start color
                        double transitionProgress = double(progress - progressPhaseTransitionToStart) / (progressPhaseStartColor2 - progressPhaseTransitionToStart);
                        newR = this->CurrentActionEndColor[i].red - unsigned char(transitionProgress * (this->CurrentActionEndColor[i].red - this->CurrentActionStartColor[i].red));
                        newG = this->CurrentActionEndColor[i].green - unsigned char(transitionProgress * (this->CurrentActionEndColor[i].green - this->CurrentActionStartColor[i].green));
                        newB = this->CurrentActionEndColor[i].blue - unsigned char(transitionProgress * (this->CurrentActionEndColor[i].blue - this->CurrentActionStartColor[i].blue));
                        newBr = this->CurrentActionEndColor[i].brightness - unsigned char(transitionProgress * (this->CurrentActionEndColor[i].brightness - this->CurrentActionStartColor[i].brightness));
                    } else {
                        // Phase start color 2
                        newR = this->CurrentActionStartColor[i].red;
                        newG = this->CurrentActionStartColor[i].green;
                        newB = this->CurrentActionStartColor[i].blue;
                        newBr = this->CurrentActionStartColor[i].brightness;
                    }
                }

                if (newR != this->CurrentColor[i].red || newG != this->CurrentColor[i].green || newB != this->CurrentColor[i].blue || newBr != this->CurrentColor[i].brightness) {
                    this->CurrentColor[i].red = newR;
                    this->CurrentColor[i].green = newG;
                    this->CurrentColor[i].blue = newB;
                    this->CurrentColor[i].brightness = newBr;
                    colorChanged = true;
                }
            }

            if (timePassed >= timeTotal) {
                this->animationRunning = false;
            }
            return colorChanged;
        }
    }
}


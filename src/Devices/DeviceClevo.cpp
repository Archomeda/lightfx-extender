#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include "DeviceClevo.h"

// Standard includes
#include <tchar.h>
#include <thread>

// Project includes
#include "../LightFXExtender.h"
#include "../Utils/Log.h"
#include "Libraries/ClevoKBLED.h"


using namespace std;
using namespace lightfx::devices::proxies;
using namespace lightfx::managers;
using namespace lightfx::timelines;
using namespace lightfx::utils;

namespace lightfx {
	namespace devices {

		void ClevoSetKBLEDThread(BlockingLightColor blocking_color, bool useTouchpad);

		BlockingLightColor color = BlockingLightColor(); // TODO: Move to INITIALIZE function

		LFXE_API bool DeviceClevo::Initialize() {
			if (!this->IsInitialized()) {

				// Start WMI Thread (Cannot be run on Main thread because of WMI CoInitializeEx issues on main thread)
				thread clevo_wmi_thread(&ClevoSetKBLEDThread, color, this->useTouchpad);
				clevo_wmi_thread.detach();

				// Gather Initialized Status
				if (color.getInitialized() == false) {
					// Mark as uninitialized
					this->SetInitialized(false);
					return false;
				}

				// Set up number of lights
				unsigned int numLights = 3; // 3 = Only Keyboard - 4 = Keyboard + Touchpad

				if (this->useTouchpad) {
					numLights = 4;
				}

				this->SetNumberOfLights(numLights);
				LOG_WARNING(to_wstring(numLights) + L" lights available");

				// Set Light Data
				LightData lightDataKBLeft;
				lightDataKBLeft.Name = L"KBLeft";
				lightDataKBLeft.Position = { 13, 2, 10 };
				this->SetLightData(0, lightDataKBLeft); // Keyboard Left

				LightData lightDataKBCenter;
				lightDataKBCenter.Name = L"KBCenter";
				lightDataKBCenter.Position = { 26, 2, 10 };
				this->SetLightData(1, lightDataKBCenter); // Keyboard Center

				LightData lightDataKBRight;
				lightDataKBRight.Name = L"KBRight";
				lightDataKBRight.Position = { 40, 2, 10 };
				this->SetLightData(2, lightDataKBRight); // Keyboard Right

				if (this->useTouchpad) {
					LightData lightDataTouchpad;
					lightDataTouchpad.Name = L"TouchPad";
					lightDataTouchpad.Position = { 20, 2, 20 };
					this->SetLightData(3, lightDataTouchpad); // Touchpad
				}

				// Mark as initialized
				this->SetInitialized(true);

			}
			return true;
		}

		LFXE_API bool DeviceClevo::Release() {
			if (this->IsInitialized()) {

				color.releaseThread(); // Set Release State

				this->SetInitialized(false);
			}
			return true;
		}

		LFXE_API bool DeviceClevo::Enable() {
			if (!this->IsEnabled()) {
				this->SetEnabled(TRUE);
				// TODO: Enable Device? Not really required.
			}
			return true;
		}

		LFXE_API bool DeviceClevo::Disable() {
			if (this->IsEnabled()) {
				this->SetEnabled(FALSE);
				// TODO: Disable Device? Turn all keys to black color could be an option. Not really required.
			}
			return true;
		}

		LFXE_API bool DeviceClevo::PushColorToDevice(const vector<LightColor>& colors) {
			color.setLightColor(colors);
			return TRUE;
		}

		// Separate Thread for WMI Interface
		void ClevoSetKBLEDThread(BlockingLightColor blocking_color, bool useTouchpad) {

			ClevoKBLED clevo = ClevoKBLED();

			// Initialize COM
			if (clevo.Initialize() != 0) {
				blocking_color.setInitialized(false);
			}else {
				blocking_color.setInitialized(true);
			}

			// Optimization Variables
			int kbleft_r =		-1;
			int kbleft_g =		-1;
			int kbleft_b =		-1;
			int kbleft_a =		-1;
			int kbcenter_r =	-1;
			int kbcenter_g =	-1;
			int kbcenter_b =	-1;
			int kbcenter_a =	-1;
			int kbright_r =		-1;
			int kbright_g =		-1;
			int kbright_b =		-1;
			int kbright_a =		-1;
			//if (useTouchpad) {
				int tp_r =		-1;
				int tp_g =		-1;
				int tp_b =		-1;
				int tp_a =		-1;
			//}

			// Main Loop
			while (true) {

				const vector<LightColor> colors = blocking_color.getLightColor(); // Blocking structure blocks the thread execution till it receives a new color

				// Release Thread
				if (blocking_color.getReleaseThreadState() == true) {
					clevo.Release(); // Release
					return; // Close Thread
				}

				// Left KB Colors
				if (kbleft_r != colors[0].red || kbleft_g != colors[0].green || kbleft_b != colors[0].blue || kbleft_a != colors[0].brightness) {
					clevo.SetKBLED(ColorKBLeft, colors[0].red, colors[0].green, colors[0].blue, (colors[0].brightness / 255.0));
					kbleft_r = colors[0].red;
					kbleft_g = colors[0].green;
					kbleft_b = colors[0].blue;
					kbleft_a = colors[0].brightness;
				}
				

				// Center KB Colors
				if (kbcenter_r != colors[1].red || kbcenter_g != colors[1].green || kbcenter_b != colors[1].blue || kbcenter_a != colors[1].brightness) {
					clevo.SetKBLED(ColorKBCenter, colors[1].red, colors[1].green, colors[1].blue, (colors[1].brightness / 255.0));
					kbcenter_r = colors[1].red;
					kbcenter_g = colors[1].green;
					kbcenter_b = colors[1].blue;
					kbcenter_a = colors[1].brightness;
				}

				// Right KB Colors
				if (kbright_r != colors[2].red || kbright_g != colors[2].green || kbright_b != colors[2].blue || kbright_a != colors[2].brightness) {
					clevo.SetKBLED(ColorKBRight, colors[2].red, colors[2].green, colors[2].blue, (colors[2].brightness / 255.0));
					kbright_r = colors[2].red;
					kbright_g = colors[2].green;
					kbright_b = colors[2].blue;
					kbright_a = colors[2].brightness;
				}

				// Touchpad Colors
				if (useTouchpad) {
					if (tp_r != colors[3].red || tp_g != colors[3].green || tp_b != colors[3].blue || tp_a != colors[3].brightness) {
						clevo.SetKBLED(ColorTouchpad, colors[3].red, colors[3].green, colors[3].blue, (colors[3].brightness / 255.0));
						tp_r = colors[3].red;
						tp_g = colors[3].green;
						tp_b = colors[3].blue;
						tp_a = colors[3].brightness;
					}
				}
			}
		}
	}
}

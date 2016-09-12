#pragma once

#include <iostream>
#include <WbemCli.h>  
#include <atlbase.h>
#include <comutil.h>

#include "../../Timelines/LightColor.h"
#include "../../Utils/BlockingLightColor.h"

using namespace std;
using namespace lightfx::timelines;

// Enums
enum KBLEDAREA : uint8_t {
	ColorKBLeft = 0xF0,
	ColorKBCenter = 0xF1,
	ColorKBRight = 0xF2,
	ColorTouchpad = 0xF3
};

enum KBLEDMODE : uint32_t {
	KBLEDON = 0x00001000,
	KBLEDOFF = 0x0000A000,
	FXDance = 0x80000000,
	FXBreath = 0x30000000,
	FXBlink = 0xA0000000,
	FXRandom = 0x90000000,
	FXSweep = 0xB0000000
};

// Class Definition
class ClevoKBLED {
	public:
		HRESULT Initialize();
		HRESULT Release();
		HRESULT SetKBLEDMode(KBLEDMODE mode);
		HRESULT SetKBLED(uint32_t hex_value);
		HRESULT SetKBLED(KBLEDAREA area, uint8_t color_r, uint8_t color_g, uint8_t color_b);
		HRESULT SetKBLED(KBLEDAREA area, uint8_t color_r, uint8_t color_g, uint8_t color_b, double alpha);

	private:
		int ToInt(double x);

		IWbemServices* pService = NULL;
		IWbemClassObject *paramsInst = NULL;
		VARIANT this_pointer;
};
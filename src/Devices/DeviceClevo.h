#pragma once

// Project includes
#include "Device.h"
#include "Libraries/ClevoKBLED.h"
#include "../Utils/BlockingLightColor.h"

// API exports
#include "../Common/ApiExports.h"

#pragma warning(push)
#pragma warning(disable : 4251)

using namespace lightfx::utils;

namespace lightfx {
	namespace devices {

		class LFXE_API DeviceClevo : public Device {

		public:
			virtual bool Initialize() override;
			virtual bool Release() override;

			virtual bool Enable() override;
			virtual bool Disable() override;

			virtual const std::wstring GetDeviceName() override { return L"Clevo"; }
			virtual const DeviceType GetDeviceType() override { return DeviceType::DeviceKeyboard; }

		protected:
			virtual bool PushColorToDevice(const std::vector<timelines::LightColor>& colors) override;

		private:
			bool useTouchpad = true;
		};

	}
}

#pragma warning(pop)
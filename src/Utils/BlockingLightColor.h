#pragma once

// Standard includes
#include <string>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>

// API exports
#include "../Common/ApiExports.h"
#include "../Timelines/LightColor.h"

using namespace std;
using namespace lightfx::timelines;

namespace lightfx {
    namespace utils {
		class BlockingLightColor {
		public:
			void setLightColor(const vector<LightColor>& colors);
			const vector<LightColor> getLightColor();

			void setInitialized(bool initialized);
			bool getInitialized();
			void resetInitialized();

			void releaseThread();
			bool getReleaseThreadState();

		private:
			vector<LightColor> color;
		};
      
    }
}

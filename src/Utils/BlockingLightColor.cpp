#ifndef LFXE_EXPORTS
#define LFXE_EXPORTS
#endif

#include <vector>
#include <condition_variable>
#include <mutex>

#include "BlockingLightColor.h"
#include "../Timelines/LightColor.h"

using namespace std;
using namespace lightfx::timelines;

mutex  color_m;
mutex  modified_m;
mutex  threadstate_m;

mutex  m;
unique_lock<mutex> data_lock(m);
condition_variable cv;

vector<LightColor> color_vector; // TODO: This is probably static, and not the best solution. How to use the private variable color? 

bool modified = false; // Variable to check if value got modified while color update
bool releaseThreadState = false;

namespace lightfx {
    namespace utils {

		void BlockingLightColor::setLightColor(const vector<LightColor>& colors) {
			// Set Color
			color_m.lock();
			color_vector = colors;
			// this->color = colors; // Doesnt works? Or class instance is not passing between calls
			color_m.unlock();

			// Modified Status
			modified_m.lock();
			modified = true;
			modified_m.unlock();

			// Notify Thread
			cv.notify_all();
		}

		// Get Light Color with Blocking Behaviour
		const vector<LightColor> BlockingLightColor::getLightColor() {
			modified_m.lock();
			if ( modified == true ) {
				modified = false;
				modified_m.unlock();
			}else {
				modified_m.unlock();
				cv.wait(data_lock);
			}
			//return this->color; // Doesnt works? Or class instance is not passing between calls
			return color_vector;
		}

		// Mark Thread as Released
		void BlockingLightColor::releaseThread() {
			// Mark Thread as Released
			threadstate_m.lock();
			releaseThreadState = true;
			threadstate_m.unlock();

			// Modified Status
			modified_m.lock();
			modified = true;
			modified_m.unlock();

			// Notify Thread
			cv.notify_all();
		}

		// Get Thread Release State
		bool BlockingLightColor::getReleaseThreadState() {
			return releaseThreadState;
		}
    }
}

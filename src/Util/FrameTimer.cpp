#include "Util/FrameTimer.hpp"
#include <thread>

namespace Util
{
	double FrameTimer::getSeconds() const {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e9;
	}

	void FrameTimer::sleepForFPS(const unsigned int FPS) const {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000/FPS) - std::chrono::nanoseconds(static_cast<std::size_t>(1e9*getSeconds())));
	}
}

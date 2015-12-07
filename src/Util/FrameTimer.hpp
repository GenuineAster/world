#pragma once
#include <chrono>

namespace Util
{
	struct FrameTimer
	{
		double sum = 0.0;
		std::size_t count = 0;
		std::chrono::high_resolution_clock::time_point start, end;
		double getSeconds() const;
		void sleepForFPS(const unsigned int FPS) const;
	};
}

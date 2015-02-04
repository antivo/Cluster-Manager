#ifndef CONFIGURATION_CLOCK
#define CONFIGURATION_CLOCK

#include <chrono>

namespace configuration {
	constexpr auto TICKS_BEFORE_SLEEP = 5;
	constexpr std::chrono::seconds SLEEP_DURATION(5);
}

#endif
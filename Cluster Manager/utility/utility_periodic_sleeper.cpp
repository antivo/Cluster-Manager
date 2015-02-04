#include "utility_periodic_sleeper.h"

#include <thread>

#include "configuration_clock.h"

namespace utility {
	std::unique_ptr<utility::PeriodicSleeper> makeDefault() {
		return std::make_unique<utility::PeriodicSleeper>(configuration::SLEEP_DURATION, configuration::TICKS_BEFORE_SLEEP);
	}

	PeriodicSleeper::PeriodicSleeper(const std::chrono::seconds & sleepDuration, const int ticksBeforeSleeping) :
		sleepDuration(sleepDuration), ticksBeforeSleeping(ticksBeforeSleeping) {}

	PeriodicSleeper::~PeriodicSleeper() {}

	void PeriodicSleeper::trySleeping() {
		--countBeforeSleep;
		if (0 <= countBeforeSleep) {
			countBeforeSleep = ticksBeforeSleeping;
		}
		std::this_thread::sleep_for(sleepDuration);
	}

}

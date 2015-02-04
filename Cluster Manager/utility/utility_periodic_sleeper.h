#ifndef UTILITY_PERIODIC_SLEEPER
#define UTILITY_PERIODIC_SLEEPER

#include <chrono>
#include <memory>

namespace utility {
	class PeriodicSleeper {
	public:
		static std::unique_ptr<utility::PeriodicSleeper> makeDefault();

		PeriodicSleeper(const std::chrono::seconds& sleepDuration, const int ticksBeforeSleeping);
		~PeriodicSleeper();

		void trySleeping();

	private:
		const std::chrono::seconds& sleepDuration;
		const int ticksBeforeSleeping;
		int countBeforeSleep;

		PeriodicSleeper(const PeriodicSleeper&) = delete;
		PeriodicSleeper operator=(const PeriodicSleeper&) = delete;
	};
}

#endif
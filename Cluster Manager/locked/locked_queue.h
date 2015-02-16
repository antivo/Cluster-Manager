#ifndef LOCKED_QUEUE
#define LOCKED_QUEUE

#include <memory>
#include <mutex>
#include <queue>

namespace locked {
	template <typename T>
	class Queue {
	public:
		Queue() {}
		~Queue() {}

		void push_back(std::unique_ptr<T>&& unfinishedJob) {
			std::lock_guard<std::mutex> guard(mutex);
			lockedQueue.emplace(std::move(unfinishedJob));
		}

		std::unique_ptr<T> pop_front() {
			std::lock_guard<std::mutex> guard(mutex);
			auto front = std::move(lockedQueue.front());
			lockedQueue.pop();
			return front;
		}

		void clear() {
			std::lock_guard<std::mutex> guard(mutex);
			lockedQueue.swap(UnfinishedJobQueue());
		}

		std::size_t size() {
			std::lock_guard<std::mutex> guard(mutex);
			return lockedQueue.size();
		}

	private:
		using LockedQueue = std::queue<std::unique_ptr<T>>;

		std::mutex mutex;
		LockedQueue lockedQueue;

		Queue(const Queue&) = delete;
		Queue& operator=(const Queue&) = delete;
	};

}

#endif
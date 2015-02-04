#ifndef ENTITY_WORKER_QUEUE
#define ENTITY_WORKER_QUEUE

#include "locked_queue.h"
#include "entity_worker.h"

namespace locked {
	using WorkerQueue = locked::Queue<entity::Worker>;
}
#endif
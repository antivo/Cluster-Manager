#ifndef ENTITY_EXECUTED_JOB_QUEUE
#define ENTITY_EXECUTED_JOB_QUEUE

#include "locked_queue.h"
#include "entity_executed_job.h"

namespace locked {
	using ExecutedJobQueue = locked::Queue<entity::ExecutedJob>;
}
#endif
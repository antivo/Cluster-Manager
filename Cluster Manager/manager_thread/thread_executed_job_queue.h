#ifndef THREAD_EXECUTED_JOB_QUEUE
#define THREAD_EXECUTED_JOB_QUEUE

#include "locked_queue.h"
#include "thread_executed_job.h"

namespace thread {
	using ExecutedJobQueue = locked::Queue<thread::ExecutedJob>;
}
#endif
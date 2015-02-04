#ifndef ENTITY_JOB_TYPE
#define ENTITY_JOB_TYPE

#include <string>

namespace entity {
	enum class JobType {
		SERIAL, PARALLEL
	};

	JobType createJobType(const std::string& jobType);
	JobType createJobType(const char jobType);	

	std::string jobTypeToString(const JobType& jobType);

}

#endif
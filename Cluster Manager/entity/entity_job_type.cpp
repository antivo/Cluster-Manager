#include "entity_job_type.h"

#include <string>

#include "assert_common.h"

namespace entity {
	JobType createJobType(const std::string& jobType) {
		assert::runtime(1 == jobType.size(), "Unsupported JobType. JobType are represented with single character. Argument provided " + jobType);
		char c = jobType[0];
		return createJobType(c);
	}

	JobType createJobType(const char jobType) {
		switch (toupper(jobType)) {
		case 'S': return JobType::SERIAL;
		case 'P': return JobType::PARALLEL;
		default: assert::runtime(false, "Unsupported JobType for char " + jobType);
		}
	}
	
	std::string jobTypeToString(const JobType& jobType) {
		switch(jobType) {
		case JobType::SERIAL: return "S";
		case JobType::PARALLEL: return "P";
		default: assert::runtime(false, "Unsupported operation. No conversion for current type");
		}
	}



}

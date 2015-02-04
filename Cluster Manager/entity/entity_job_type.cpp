#include "entity_job_type.h"

#include <string>
#include <cctype>
#include <stdexcept>

namespace entity {
	JobType createJobType(const std::string& jobType) {
		if (1 != jobType.size()) {
			throw std::runtime_error("Unsupported JobType. JobType are represented with single character. Argument provided " + jobType);
		}
		
		char c = jobType[0];
		return createJobType(c);
	}

	JobType createJobType(const char jobType) {
		switch (toupper(jobType)) {
		case 'S': return JobType::SERIAL;
		case 'P': return JobType::PARALLEL;
		default: throw ::std::runtime_error("Unsupported JobType for char " + jobType);
		}
	}
	
	std::string jobTypeToString(const JobType& jobType) {
		switch(jobType) {
		case JobType::SERIAL: return "S";
		case JobType::PARALLEL: return "P";
		default: throw new std::runtime_error("Unsupported operation. No conversion for current type ");
		}
	}



}

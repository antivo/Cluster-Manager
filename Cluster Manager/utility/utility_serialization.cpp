#include "utility_serialization.h"

#include "configuration_default.h"
#include "utility_string.h"

namespace utility {
	std::string serialise(const std::vector<std::string>& stringVector) {
		return utility::concatenateWithStartingDelimiter(stringVector, configuration::DEFAULT_DELIMITER);
	}

	std::vector<std::string> deserialize(const std::string& serializedStringVector) {
		const auto delimiter = serializedStringVector[0];
		return utility::stringExplode(serializedStringVector, delimiter);
	}
}
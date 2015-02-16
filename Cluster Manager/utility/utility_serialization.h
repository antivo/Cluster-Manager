#ifndef UTILITY_SERIALIZATION
#define UTILITY_SERIALIZATION

#include <string>
#include <vector>

namespace utility {
	std::string serialise(const std::vector<std::string>& stringVector);
	std::vector<std::string> deserialize(const std::string& serializedStringVector);
}

#endif

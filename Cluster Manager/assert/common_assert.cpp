#include "common_assert.h"

void assert::condition(const bool condition, const std::string& message) {
	if (!condition) {
		throw std::exception(message.c_str());
	}
}

void assert::stringNotEmpty(const std::string& property, const std::string& message) {
	assert::condition(!property.empty(), message);
}


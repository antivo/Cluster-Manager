#ifndef ASSERT_COMMON_ASSERT
#define ASSERT_COMMON_ASSERT

#include <string>

namespace assert {
	void condition(const bool condition, const std::string& message);

	void stringNotEmpty(const std::string& property, const std::string& message);
}

#endif
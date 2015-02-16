#ifndef ASSERT_COMMON
#define ASSERT_COMMON

#include <functional>
#include <string>

namespace assert {
	void condition(const bool condition, const std::function<std::exception(void)>& generateException);

	void runtime(const bool condition, const std::string& message);

	void stringNotEmpty(const std::string& property, const std::string& message);
}

#endif
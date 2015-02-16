#include "assert_common.h"

namespace assert {
	void condition(const bool condition, const std::function<std::exception(void)>& generateException) {
		if (!condition) {
			throw generateException();
		}
	}

	void runtime(const bool condition, const std::string& message) {
		assert::condition(condition, [&]() { return std::runtime_error(message.c_str()); });
	}

	void stringNotEmpty(const std::string& property, const std::string& message) {
		assert::runtime(!property.empty(), message);
	}
}


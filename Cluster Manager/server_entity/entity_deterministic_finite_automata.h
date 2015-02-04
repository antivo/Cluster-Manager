#ifndef SERVER_ENTITY_DETERMINISTIC_FINITE_AUTOMATA
#define SERVER_ENTITY_DETERMINISTIC_FINITE_AUTOMATA

#include <string>
#include <functional>
#include <map>

namespace entity {
	class ClientRecord;
}

namespace entity {
	class DeterministicFiniteAutomata {
	public:
		DeterministicFiniteAutomata(const std::map<std::string, const std::function<std::string(entity::ClientRecord&, const std::string&)>>& transitionFunction);
		~DeterministicFiniteAutomata();

		std::string transit(entity::ClientRecord& data, const std::string& input) const;

	private:
		const std::map<std::string, const std::function<std::string(entity::ClientRecord&, const std::string&)>> transitionFunction;

		DeterministicFiniteAutomata(const DeterministicFiniteAutomata&) = delete;
		DeterministicFiniteAutomata& operator=(const DeterministicFiniteAutomata&) = delete;
	};
}

#endif
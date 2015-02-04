#include "entity_deterministic_finite_automata.h"

#include "entity_client_record.h"

namespace entity {
	DeterministicFiniteAutomata::DeterministicFiniteAutomata(const std::map<std::string, const std::function<std::string(entity::ClientRecord&, const std::string&)>>& transitionFunction) :
		transitionFunction(transitionFunction) {}

	DeterministicFiniteAutomata::~DeterministicFiniteAutomata() {}

	std::string DeterministicFiniteAutomata::transit(entity::ClientRecord& data, const std::string& input) const {
		auto state = data.getState();
		auto transition = transitionFunction.at(state);
		return transition(data, input);
	}
}
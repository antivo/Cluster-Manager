#include "entity_client_record.h"

namespace entity {
	std::string ClientRecord::getStartingState() {
		return "";
	}

	ClientRecord::ClientRecord() : state(getStartingState()) {}

	ClientRecord::~ClientRecord() {}

	std::string ClientRecord::getState() const {
		return state;
	}

	void ClientRecord::setState(const std::string& newState) {
		state = newState;
	}

	std::shared_ptr<entity::Contract> ClientRecord::getContract() const {
		return this->contract;
	}

	void ClientRecord::setContract(const std::shared_ptr<entity::Contract>& sharedContract) {
		this->contract = contract;
	}

	std::string ClientRecord::getProperty(const std::string& propertyName) const {
		return properties.at(propertyName);
	}

	void ClientRecord::eraseProperty(const std::string& propertyName) {
		properties.erase(propertyName);
	}

	void ClientRecord::setProperty(const std::string& propertyName, const std::string& properyValue) {
		properties[propertyName] = properyValue;
	}
}
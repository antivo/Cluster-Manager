#include "entity_raw_contract.h"

#include <algorithm>

#include "assert_common.h"
#include "configuration_default.h"
#include "entity_contract.h"
#include "utility_string.h"

namespace entity {
	RawContract::RawContract(std::vector<std::string>&& constractStringVector) : constractStringVector(constractStringVector) {
		assertRawContract();
	}

	RawContract::RawContract(const std::vector<std::string>& constractStringVector) : constractStringVector(constractStringVector) {
		assertRawContract();
	}

	RawContract::~RawContract() {}

	std::shared_ptr<Contract> RawContract::getContract() {
		if (!contract) {
			rafine();
		}
		return contract;
	}

	std::shared_ptr<std::vector<std::string>> RawContract::getLocalPathList() {
		if (!localPathList) {
			rafine();
		}
		return localPathList;
	}

	void RawContract::rafine() {
		this->localPathList = std::make_shared<std::vector<std::string>>();
		for (int i = entity::Contract::PathListStart; i < this->constractStringVector.size(); ++i) {
			auto& field = this->constractStringVector[i];
			auto parts = utility::stringExplode(field, configuration::DEFAULT_SEPARATOR);
			assert::runtime(parts.size() == 2, "line " + field + "has more than one separator '" + configuration::DEFAULT_SEPARATOR + "'");
			std::for_each(parts.begin(), parts.end(), utility::trim);
			localPathList->emplace_back(std::move(parts[0]));
			field = std::move(parts[1]);
		}
		this->contract = std::make_shared<entity::Contract>(std::move(constractStringVector));
	}

	void RawContract::assertRawContract() const {
		const auto args = this->constractStringVector[entity::Contract::Args];
		assert::runtime(std::string::npos == args.find(configuration::DEFAULT_DELIMITER), "Contract file must not have character '#' in its argument line");
	}
}

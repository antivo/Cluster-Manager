#ifndef ENTITY_RAW_CONTRACT
#define ENTITY_RAW_CONTRACT

#include <memory>
#include <string>
#include <vector>

/// can not be used across multiple threads
namespace entity {
	class Contract;

	class RawContract {
	public:
		explicit RawContract(std::vector<std::string>&& constractStringVector);
		explicit RawContract(const std::vector<std::string>& constractStringVector);
		~RawContract();

		std::shared_ptr<Contract> getContract();
		std::shared_ptr<std::vector<std::string>> getLocalPathList();
	
	private:
		std::vector<std::string> constractStringVector;
		std::shared_ptr<Contract> contract;
		std::shared_ptr<std::vector<std::string>> localPathList;

		void rafine();
		void assertRawContract() const;
	};
}

#endif
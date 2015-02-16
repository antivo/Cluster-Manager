#ifndef ENTITY_CLIENT_RECORD
#define ENTITY_CLIENT_RECORD

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace entity {
	class Contract;
}

namespace entity {
	class ClientRecord {
	public:
		static std::string getStartingState();

		ClientRecord();
		//ClientRecord(const ClientRecord& other);
		~ClientRecord();

		std::string getState() const;
		void setState(const std::string& newState);

		std::shared_ptr<entity::Contract> getContract() const;
		void setContract(const std::shared_ptr<entity::Contract>& contract);

		std::shared_ptr<std::vector<std::string>> getResultTopology();
		void setResultTopology(const std::shared_ptr<std::vector<std::string>>& resultTopology);

		/// <exception cref="std::out_of_range">Thrown when has no property by the given name.</exception>
		std::string getProperty(const std::string& propertyName) const;
		void eraseProperty(const std::string& propertyName);
		void setProperty(const std::string& propertyName, const std::string& properyValue);

	private:
		std::string state;
		std::shared_ptr<entity::Contract> contract;
		std::shared_ptr<std::vector<std::string>> resultTopology;
		std::map<std::string, std::string> properties;
	};
}

#endif
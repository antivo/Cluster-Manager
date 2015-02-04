#ifndef ENTITY_CONTRACT
#define ENTITY_CONTRACT

#include <string>
#include <vector>

namespace entity {

	class Contract {
	public:
		Contract(const std::vector<std::string>& contractVector);
		Contract(std::vector<std::string>&& contractVector);

		std::string getJobName() const;
		std::string getJobType() const;
		std::string getNeededWorkers() const;
		std::string getArgs() const;
		std::string getExecutable() const;
		std::vector<std::string> getPathList() const;

		std::vector<std::string> getContractVector() const;
	
	private:
		std::vector<std::string> contractVector;

		static bool isExecutable(const std::string& filename);
		void assertContract();

		enum ContractVectorField {
			JobName = 0, JobType = 1, NeededWorkers = 2, Args = 3, Executable = 4, PathListStart = 4
		};
	};

}

#endif
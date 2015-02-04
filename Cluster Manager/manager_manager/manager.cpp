#include "manager.h"

namespace manager {
	Manager::Manager(std::shared_ptr<persistent::ManagerData> persistentData):
		persistentData{persistentData} 
	{
	}

	void Manager::executeOrder(const std::string& order) {
			
	}
}

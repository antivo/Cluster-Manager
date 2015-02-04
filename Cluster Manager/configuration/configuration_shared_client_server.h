#ifndef CONFIGURATION_SHARED_CLIENT_SERVER
#define CONFIGURATION_SHARED_CLIENT_SERVER

#include <string>
#include <memory>

namespace configuration {
	
	class SharedClientServer {
	public:
		static std::shared_ptr<configuration::SharedClientServer> makeDefault();
		
		SharedClientServer(std::string successMessage, std::string errorMessage);
		~SharedClientServer();

		std::string getSucessMessage() const;
		std::string getErrorMessage() const;
		
		void setSucessMessage(const std::string& successMessage);
		void setErrorMessage(const std::string& errorMessage);
	
	private:
		std::string successMessage;
		std::string errorMessage;
	};

}

#endif
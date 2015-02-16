#ifndef CONFIGURATION_SHARED_CLIENT_SERVER
#define CONFIGURATION_SHARED_CLIENT_SERVER

#include <memory>
#include <string>

namespace configuration {
	class SharedClientServer {
	public:
		static std::shared_ptr<configuration::SharedClientServer> makeDefault();
		
		SharedClientServer(const unsigned short port,
											 const std::string& successMessage, 
											 const std::string& errorMessage,
											 const std::string& clientChoiceSchedule,
											 const std::string& clientChoiceObtain,
											 const std::string& clientChoiceInfo,
											 const std::string& clientChoiceLogout);
		~SharedClientServer();

		unsigned int getPort() const;
		std::string getSucessMessage() const;
		std::string getErrorMessage() const;
		std::string getClientChoiceSchedule() const;
		std::string getClientChoiceObtain() const;
		std::string getClientChoiceInfo() const;
		std::string getClientChoiceLogout() const;
		
		void setPort(unsigned int port);
		void setSucessMessage(const std::string& successMessage);
		void setErrorMessage(const std::string& errorMessage);
		void setClientChoiceSchedule(const std::string& clientChoiceSchedulee);
		void setClientChoiceObtain(const std::string& clientChoiceObtain);
		void setClientChoiceInfo(const std::string& clientChoiceInfo);
		void setClientChoiceLogout(const std::string& clientChoiceLogout);

	private:
		unsigned short port;
		std::string successMessage;
		std::string errorMessage;
		std::string clientChoiceSchedule;
		std::string clientChoiceObtain;
		std::string clientChoiceInfo;
		std::string clientChoiceLogout;
	};

}

#endif
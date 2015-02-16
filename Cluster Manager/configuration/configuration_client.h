#ifndef CONFIGURATION_CLIENT
#define CONFIGURATION_CLIENT

#include <memory>
#include <string>

namespace configuration {
	class Client {
	public:
		static std::unique_ptr<configuration::Client> makeDefault();

		Client(const int consoleColor, const std::string& serverAddress);
		~Client();

		int getConsoleColor() const;
		std::string getServerAddress() const;
		
		void setConsoleColor(const int consoleColor);
		void setServerAddress(const std::string& serverAddress);

	private:
		int consoleColor;
		std::string serverAddress;
	};
}

#endif
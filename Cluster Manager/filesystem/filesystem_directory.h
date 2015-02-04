#ifndef FILESYSTEM_DIRECTORY
#define FILESYSTEM_DIRECTORY

#include <string>
#include <memory>
#include <vector>

namespace filesystem {
	class Directory {
	public:
		Directory(const std::string path);
		~Directory();

		Directory makeSubdirectory(const std::string& name) const;
		void store(const std::string& relativePath, const std::string& content) const;
		void copy(const Directory& destination) const;
		/// doesn't work on the local network between computers
		void move(const Directory& destination);
		/// copy and remove works like moving in local network between computers
		void copyAndRemove(const Directory& destination);
		void remove();

		std::string getPath() const;
		std::string appendToPath(const std::string& suffix) const;
		
		std::vector<std::string> createTopology() const;

	private:
		std::string path;
	};
}

#endif
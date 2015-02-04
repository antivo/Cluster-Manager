#ifndef FILESYSTEM_DIRECTORY_COMMON
#define FILESYSTEM_DIRECTORY_COMMON

#include <string>
#include <vector>

namespace filesystem {
  namespace directory {
		bool exists(const std::string& path);
		/// creates full path (all down to the leaf folder), throws exception
    void create(const std::string& path); 
		/// throws if no destination exist
    bool remove(const std::string& path);

		void copy(const std::string& src, const std::string& dst);
		std::vector<std::string> getTopology(const std::string& src);
		void move(const std::string& src, const std::string& dst);


		/// must end with backslash for directory
		static void recursiveCopy(const char* r_szSrcPath, const char* r_szDesPath);
		/// must end with backslash for directory
		static void generateTopology(const char* r_szSrcPath, std::vector<std::string>& topology);
	}
}

#endif
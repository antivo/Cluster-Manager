#ifndef FILESYSTEM_FILE_COMMON
#define FILESYSTEM_FILE_COMMON

#include <string>

namespace filesystem {
  namespace file {
    //bool exists(const std::string& path);
		/// throws
    void create(const std::string& path, const std::string& content); 
		void copy(const std::string& src, const std::string& dst);
		//void remove(const std::string& path); 
    //std::string read(const std::string path);
  }
}

#endif
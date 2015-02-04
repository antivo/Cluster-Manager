#include "filesystem_file_common.h"

#include <windows.h>

#include <fstream>

namespace filesystem {
  namespace file {
    void create(const std::string&path, const std::string& content) {
      std::ofstream outfile(path);
      outfile.write(content.c_str(), content.size());
    }

		void copy(const std::string& src, const std::string& dst) {
			CopyFileA(src.c_str(), dst.c_str(), TRUE);
		}
  }
}
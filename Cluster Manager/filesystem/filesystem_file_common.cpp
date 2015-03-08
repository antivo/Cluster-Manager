#include "filesystem_file_common.h"

#include <windows.h>

#include <fstream>

namespace filesystem {
  namespace file {
    void create(const std::string& path, const std::string& content) {
			if (content.size()) { // assert ??
				std::ofstream outfile(path, std::ofstream::out | std::ostream::binary);
				outfile.write(content.c_str(), content.size());
				outfile.close();
			}
    }

		void copy(const std::string& src, const std::string& dst) {
			CopyFileA(src.c_str(), dst.c_str(), TRUE);
		}
  }
}
#ifndef FILESYSTEM_COMMON
#define FILESYSTEM_COMMON

#include <string>
#include <vector>

namespace filesystem {
	static constexpr auto pathDelimiter = R"(\)";

	std::string addToPath(const std::string& path, const std::string& append);
	std::string getDirectoryPathOf(const std::string& path);

	bool isDirectory(const std::string& name);
  bool endsWithExe(const std::string& filename);
  bool isFilePath(const std::string& FilePath);
}

#endif
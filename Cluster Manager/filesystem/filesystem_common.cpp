#include "filesystem_common.h"

#include <windows.h>

#include <regex>
#include <sstream>

#include "utility_string.h"

namespace filesystem {
  std::string addToPath(const std::string& path, const std::string& append) {
		std::stringstream ss;
		ss << path << pathDelimiter << append;
		return ss.str();
	}

	bool isDirectory(const std::string& name) {
    std::regex windowsDirectory(R"DELIMITER(^(?!(?:CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\.[^.]*)?$)[^<>:"/\\|?*\x00-\x1F]*[^<>:"/\\|?*\x00-\x1F\ .]$)DELIMITER");
    return std::regex_match(name, windowsDirectory);
  }

  bool endsWithExe(const std::string& filename) {
    const auto lastDot = filename.rfind('.');
    if(std::string::npos != lastDot) {
      return 0 == filename.substr(lastDot + 1).compare("exe");
    }
    return false;
  }

  bool isFilePath(const std::string& filePath) {
    const auto names = utility::stringExplode(filePath, R"(/\)");
    auto size = names.size();
    auto retValue = true;
    for(decltype(size) i = 0; retValue && i < size; ++i) {
      retValue = retValue && isDirectory(names[i]);
    }
    return retValue;
  }

}
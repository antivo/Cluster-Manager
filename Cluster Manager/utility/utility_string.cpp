#include "utility_string.h"


#include <fstream>
#include <sstream>
#include <streambuf>
#include <regex>


namespace utility {
  std::string concatenateWithStartingDelimiter(const std::vector<std::string>& lines, const char delimiter) {
    std::stringstream sstream;
    auto size = lines.size();
    for(decltype(size) i = 0; i < size; ++i) {
      sstream << delimiter << lines[i];
    }
    return sstream.str();
  }

  std::size_t findNthOccurrence(const std::string& input, const char delimiter, const std::size_t& occurrence) {
    std::size_t index = std::string::npos;
    for(decltype(index) i = 0; i < occurrence; ++i) {
      index = input.find(delimiter, index + 1);
      if(std::string::npos == index) {
        break;
      }
    }
    return index;
  }

  std::vector<std::string> stringExplode(const std::string& inputString, const char separator) {
    auto str = inputString;
    auto found = str.find(separator);
    std::vector<std::string> results;
    while(std::string::npos != found) {
      if(0 < found) {
        results.push_back(str.substr(0, found));
      }
      str = str.substr(found+1);
      found = str.find(separator);
    }
    if(0 < str.length()) {
      results.push_back(str);
    }
    return results;
  }

  std::vector<std::string> stringExplode(const std::string& inputString, const std::string& separatorList) {
    auto str = inputString;
    auto found = str.find_first_of(separatorList);
    std::vector<std::string> results;
    while(found != std::string::npos) {
      if(found > 0) {
        results.push_back(str.substr(0, found));
      }
      str = str.substr(found+1);
      found = str.find_first_of(separatorList);
    }
    if(0 < str.length()) {
      results.push_back(str);
    }
    return results;
  }

  bool isNonZeroNonNegativeInt(const std::string& str) {
    std::regex acceptable("[1-9][[:digit:]]*");
    return std::regex_match(str, acceptable);
  }

	/*std::string addSlashes(const std::string& xs) {
		std::stringstream ss;
		for (const auto c : xs) {
			ss << c;
			if ('\\' == c) {
				ss << '\\';
			}
		}
	}*/

	std::string fileToString(std::string& filepath) {
		std::ifstream t(filepath.c_str());
		std::string ss;

		t.seekg(0, std::ios::end);
		ss.reserve(static_cast<int>(t.tellg()));
		t.seekg(0, std::ios::beg);

		ss.assign((std::istreambuf_iterator<char>(t)),
							 std::istreambuf_iterator<char>());
		return ss;
	}
}
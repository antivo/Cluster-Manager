#ifndef UTILITY_STRING
#define UTILITY_STRING

#include <string>
#include <vector>

namespace utility {
  std::string concatenateWithStartingDelimiter(const std::vector<std::string>& lines, const char delimiter);

	//std::string addSlashes(const std::string& xs);

  /// returns std::string::npos if there is no n-th occurrence
  std::size_t findNthOccurrence(const std::string& input, const char delimiter, const std::size_t& occurrence);
  
  std::vector<std::string> stringExplode(const std::string& inputString, const char separator);
  std::vector<std::string> stringExplode(const std::string& inputString, const std::string& separatorList);

  bool isNonZeroNonNegativeInt(const std::string& str);

	std::string fileToString(std::string& filepath);
}

#endif
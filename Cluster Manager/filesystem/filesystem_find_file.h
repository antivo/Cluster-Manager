#ifndef FILESYSTEM_FIND_FILE
#define FILESYSTEM_FIND_FILE

#include <windows.h>
#include <string>
#include <memory>

namespace filesystem {

  class FindFile {
  public:
    FindFile(const std::string& lpFileName);
    ~FindFile();

    bool findNextFile() const;
    std::string getName() const;
    DWORD getAttributes() const;

  private:
    std::unique_ptr<WIN32_FIND_DATAA> findFileData;
    const HANDLE hFind;

    FindFile(const FindFile&) = delete;
    FindFile& operator=(const FindFile&) = delete;
  };

}

#endif
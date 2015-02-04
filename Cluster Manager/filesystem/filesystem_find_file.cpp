#include "filesystem_find_file.h"

namespace filesystem {

  FindFile::FindFile(const std::string& lpFileName) : findFileData(std::make_unique<WIN32_FIND_DATAA>()),
                                            hFind(FindFirstFileA(lpFileName.c_str(), findFileData.get())) {
    if(INVALID_HANDLE_VALUE == hFind) {
      throw std::runtime_error("Can not find");
    }
  }
    
  FindFile::~FindFile() {
    FindClose(hFind);
  }

  bool FindFile::findNextFile() const {
    return 0 != FindNextFileA(hFind, findFileData.get());
  }
    
  std::string FindFile::getName() const{
    return findFileData->cFileName;
  }
    
  DWORD FindFile::getAttributes() const {
    return findFileData->dwFileAttributes;
  }
}
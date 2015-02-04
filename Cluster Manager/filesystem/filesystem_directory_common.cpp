#include "filesystem_directory_common.h"

#include <sys/stat.h>
#include <io.h>
#include <windows.h>

#include <sstream>

#include "common_assert.h"
#include "filesystem_find_file.h"
#include "filesystem_common.h"

namespace filesystem {
  namespace directory {
    bool exists(const std::string& path) {
      auto ftyp = GetFileAttributesA(path.c_str());
      if (INVALID_FILE_ATTRIBUTES == ftyp) {
        return false;
      }
      if (FILE_ATTRIBUTE_DIRECTORY & ftyp) {
        return true;
      }
      return false;
    }
    
    void create(const std::string& path) {
      static const auto separators = R"(/\)";
      const auto fileAttributes = ::GetFileAttributesA(path.c_str());
      if(INVALID_FILE_ATTRIBUTES == fileAttributes) {
        const auto separatorIndex = path.find_last_of(separators);
        if(std::wstring::npos != separatorIndex) {
          create(path.substr(0, separatorIndex));
        }
        const auto result = ::CreateDirectoryA(path.c_str(), nullptr);
        if(false == result) {
          throw std::runtime_error("Could not create directory");
        }
      } else {
        const auto isDirectoryOrJunction = ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
                                           ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);
        if(!isDirectoryOrJunction) {
          throw std::runtime_error("File with the same name exists");
        }
      }
    }
    
    bool remove(const std::string& path) {
      auto dirPath = path + R"(\*)";
      auto fileName = path + R"(\)";
      const auto findFile = std::make_unique<FindFile>(dirPath.c_str()); // find the first file
      dirPath = fileName;    
      auto bSearch = true;
      while(bSearch) {
        if(findFile->findNextFile()) {
          const auto cFileName = findFile->getName();
          if(cFileName.compare(".") && cFileName.compare("..")) {
            fileName.append(cFileName);
            const auto dwFileAttributes = findFile->getAttributes();
            if((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
              if(!remove(fileName)) {
								// directory couldn't be deleted
                return false; 
              }
							// remove the empty directory
              RemoveDirectoryA(fileName.c_str()); 
            } else {
              if(dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
								// change read-only file mode
                _chmod(fileName.c_str(), _S_IWRITE); 
              }
							// delete the file
              if(!DeleteFileA(fileName.c_str())) {  
                  return false; 
              }                 
            }
            fileName = dirPath;
          }
        } else {
					// no more files there
          if(ERROR_NO_MORE_FILES == GetLastError()) 
            bSearch = false;
          else {
            return false;
          }
        }
      }

			// remove the empty directory
      return 0 != RemoveDirectoryA(path.c_str()); 
    }

		void copy(const std::string & src, const std::string & dst) {
			const auto source = src + filesystem::pathDelimiter;
			const auto destination = dst + filesystem::pathDelimiter;
			filesystem::directory::recursiveCopy(source.c_str(), destination.c_str());
		}

		std::vector<std::string> getTopology(const std::string & src) {
			const auto source = src + filesystem::pathDelimiter;
			std::vector<std::string> topology;
			filesystem::directory::generateTopology(source.c_str(), topology);
			return topology;
		}

		void move(const std::string & src, const std::string & dst) {
			remove(dst);
			MoveFileExA(src.c_str(), dst.c_str(), MOVEFILE_COPY_ALLOWED);
		}
  
		/// if src and dst are directory, then they must end with pathDelimiter
		void recursiveCopy(const char* r_szSrcPath, const char* r_szDesPath) {
			WIN32_FIND_DATAA FindFileData;
			HANDLE hFind;
			char l_szTmp[256] = { 0 };
			memcpy(l_szTmp, r_szSrcPath, strlen(r_szSrcPath));
			char l_szSrcPath[256] = { 0 };
			char l_szDesPath[256] = { 0 };
			memcpy(l_szSrcPath, r_szSrcPath, strlen(r_szSrcPath));
			memcpy(l_szDesPath, r_szDesPath, strlen(r_szDesPath));
			char l_szNewSrcPath[256] = { 0 };
			char l_szNewDesPath[256] = { 0 };
			strcat_s(l_szTmp, "*");
			hFind = FindFirstFileA(l_szTmp, &FindFileData);

			if (NULL == hFind) throw new std::runtime_error("recursive copy - no file/directory found");
			do {
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (strcmp(FindFileData.cFileName, ".")) {
						if (strcmp(FindFileData.cFileName, "..")) {
							sprintf_s(l_szNewDesPath, "%s%s\\", l_szDesPath, FindFileData.cFileName);
							sprintf_s(l_szNewSrcPath, "%s%s\\", l_szSrcPath, FindFileData.cFileName);
							CreateDirectoryA(l_szNewDesPath, NULL);
							recursiveCopy(l_szNewSrcPath, l_szNewDesPath);
						}
					}
				} else {
					char l_szSrcFile[256] = { 0 };
					char l_szDesFile[256] = { 0 };
					sprintf_s(l_szDesFile, "%s%s", l_szDesPath, FindFileData.cFileName);
					sprintf_s(l_szSrcFile, "%s%s", l_szSrcPath, FindFileData.cFileName);
					CopyFileA(l_szSrcFile, l_szDesFile, TRUE);
				}
			} while (FindNextFileA(hFind, &FindFileData));
			FindClose(hFind);
		}

		void generateTopology(const char * r_szSrcPath, std::vector<std::string>& topology) {
			WIN32_FIND_DATAA FindFileData;
			HANDLE hFind;
			char l_szTmp[512] = { 0 };
			memcpy(l_szTmp, r_szSrcPath, strlen(r_szSrcPath));
			char l_szSrcPath[512] = { 0 };
			memcpy(l_szSrcPath, r_szSrcPath, strlen(r_szSrcPath));
			char l_szNewSrcPath[512] = { 0 };
			strcat_s(l_szTmp, "*");
			hFind = FindFirstFileA(l_szTmp, &FindFileData);
			if (NULL == hFind) throw new std::runtime_error("recursive copy - no file/directory found");
			do {
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (strcmp(FindFileData.cFileName, ".")) {
						if (strcmp(FindFileData.cFileName, "..")) {
							sprintf_s(l_szNewSrcPath, "%s%s\\", l_szSrcPath, FindFileData.cFileName);
							// l_szSrcPath  --  dir -- YOU CAN INCLUDE IT TO ALLOW DIRS
							generateTopology(l_szNewSrcPath, topology);
						}
					}
				} else {
					char l_szSrcFile[512] = { 0 };
					sprintf_s(l_szSrcFile, "%s%s", l_szSrcPath, FindFileData.cFileName);
					topology.push_back(l_szSrcFile);
				}
			} while (FindNextFileA(hFind, &FindFileData));
			FindClose(hFind);
		}

	}
}
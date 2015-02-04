#include <iostream>
#include <list>

int main() {
	std::list<int> mylist;
	// set some values:
	for (int i = 1; i<10; ++i) mylist.push_back(i * 10);

	auto it = mylist.begin();
	while (mylist.end() != it) {
		std::cout << *it << std::endl;
		if (*it == 20) {
			it = mylist.erase(it);
		} else {
			it = std::next(it);
		}
	}

	std::cout << std::endl;
	
	std::cout << "mylist contains:";
	for (auto it1 = mylist.begin(); it1 != mylist.end(); ++it1)
		std::cout << ' ' << *it1;
	std::cout << '\n';

	return 0;
}

/*#include <iostream>
#include <fstream>  
#include <thread>
#include <chrono>

#include "filesystem_file_common.h"

int main(int argc, char** argv) {
	std::string src(R"(C:\a\a.txt)");
	std::string dst(R"(C:\a\b.txt)");

	filesystem::file::copy(src.c_str(), dst.c_str());

	/*td::cout << "Izvodim Test.exe argc: " << argc <<std::endl;
	const std::chrono::seconds SLEEP_DURATION(30);
	std::this_thread::sleep_for(SLEEP_DURATION);
	std::cout << "zavrsio sam sa izvodenjem" << std::endl;
	
	return 0;
}

/*#include <iostream>

#include "windows.h"

#include <memory>
#include <queue>
#include <mutex>
#include <vector>

#include "filesystem_directory.h"
#include "filesystem_job_directory.h"

#include "filesystem_directory_common.h"

int main() {
	filesystem::Directory workerDirOne(R"(\\C10\mpi)");
	filesystem::Directory workerDirTwo(R"(\\C12\mpi)");

	filesystem::JobDirectory jobDirectory(R"(D:)", "client", "job");
	jobDirectory.prepareForDataIncome("data");
	jobDirectory.prepareForResultIncome("results");
	
	auto a = jobDirectory.deployData(workerDirOne, 0);
	auto b = jobDirectory.deployData(workerDirTwo, 1);

	jobDirectory.fetchResult(a);
	jobDirectory.fetchResult(b);
	

}

/*

bool recursiveCopy(const char r_szSrcPath[256], const char r_szDesPath[512]) {
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	char l_szTmp[256] = { 0 };

	memcpy(l_szTmp, r_szSrcPath, strlen(r_szSrcPath));

	char l_szSrcPath[256] = { 0 };

	char l_szDesPath[256] = { 0 };

	memcpy(l_szSrcPath, r_szSrcPath, strlen(r_szSrcPath));

	memcpy(l_szDesPath, r_szDesPath, strlen(r_szDesPath));

	char l_szNewSrcPath[256] = { 0 };

	char l_szNewDesPath[256] = { 0 };

	strcat(l_szTmp, "*");

	hFind = FindFirstFile((LPCWSTR) l_szTmp, &FindFileData);

	if (hFind == NULL) return false;

	do {

		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			if (strcmp((const char*) FindFileData.cFileName, ".")) {

				if (strcmp((const char*) FindFileData.cFileName, "..")) {

					sprintf(l_szNewDesPath, "%s%s\\", l_szDesPath, FindFileData.cFileName);

					sprintf(l_szNewSrcPath, "%s%s\\", l_szSrcPath, FindFileData.cFileName);

					CreateDirectoryA(l_szNewDesPath, NULL);

					recursiveCopy(l_szNewSrcPath, l_szNewDesPath);

				}

			}

		} else {

			char l_szSrcFile[256] = { 0 };

			char l_szDesFile[256] = { 0 };

			sprintf(l_szDesFile, "%s%s", l_szDesPath, FindFileData.cFileName);

			sprintf(l_szSrcFile, "%s%s", l_szSrcPath, FindFileData.cFileName);

			bool l_bRet = CopyFileA(l_szSrcFile, l_szDesFile, TRUE);

		}

	}

	while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);

	return true;
}

void create(const std::string& path) {
	static const auto separators = R"(/\)";
	const auto fileAttributes = ::GetFileAttributesA(path.c_str());
	if (INVALID_FILE_ATTRIBUTES == fileAttributes) {
		const auto separatorIndex = path.find_last_of(separators);
		if (std::wstring::npos != separatorIndex) {
			create(path.substr(0, separatorIndex));
		}
		const auto result = ::CreateDirectoryA(path.c_str(), nullptr);
		if (false == result) {
			throw std::runtime_error("Could not create directory");
		}
	} else {
		const auto isDirectoryOrJunction = ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
			((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);
		if (!isDirectoryOrJunction) {
			throw std::runtime_error("File with the same name exists");
		}
	}
}

bool isDirectory(const std::string& name) {
	std::regex windowsDirectory(R"DELIMITER(^(?!(?:CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\.[^.]*)?$)[^<>:"/\\|?*\x00-\x1F]*[^<>:"/\\|?*\x00-\x1F\ .]$)DELIMITER");
	return std::regex_match(name, windowsDirectory);
}

int main() {
	
	std::string src( "C:\\A" );
	std::string dst("C:\\B\\NEPOSTOJI");
	//create(dst);
	std::cout << isDirectory("NOT");
	//MoveFileA(src.c_str(), dst.c_str());
}*/

/*

#include <regex>
//#include <funct>
#include <iostream>

bool isDirectory(const std::string& name) {
	std::regex windowsDirectory(R"DELIMITER(^(?!(?:CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\.[^.]*)?$)[^<>:"/\\|?*\x00-\x1F]*[^<>:"/\\|?*\x00-\x1F\ .]$)DELIMITER");
	return std::regex_match(name, windowsDirectory);
}

int main() {

	std::cout << isDirectory("isus.jpeg");
	/*
    auto dbControler = memory::make_unique<database::ControlerServerSide>(configuration::databaseHostName, 
                                                                          configuration::databaseUserName, 
                                                                          configuration::databasePassword, 
                                                                          configuration::databaseName, 
                                                                          configuration::databasePortNumber);
		server::DetailedServer logg(configuration::serverPortInt, 
																configuration::filesystemRootDirectory, 
																configuration::filesystemDataDirectory, 
																configuration::filesystemRecvDirectory,
																configuration::treeFileName,
																configuration::sucessMessage,
																configuration::errorMessage
																);
    logg.run(
    [&](const std::string& user, const std::string& pass) -> std::string 
    {
      return dbControler->getClientID(user, pass);
    }, 
    [](const std::string& id) -> std::string 
    {
      return "ne znam ti ja nista";
    }, 
    [](const std::string& id) -> std::string 
    {
      return "ne znam ti ja nista";
    },
    [&](const std::string& id, const std::string& jobName) -> bool
    {
      auto rez = dbControler->jobExists(id,jobName);
      if(rez) {
        std::cout << "posao postoji" << std::endl;
      } else {
        std::cout << "posao NE postoji" << std::endl;
      }
      return rez;
    },
    [&](const std::string& id, 
       const std::string& job, 
       const std::string& parallel, 
       const std::string& neededWorkers, 
       const std::string& path)
    {
      std::cout << "SPREMAN > " << id << " " << job << " " << parallel << " " << neededWorkers << " " << path << std::endl; 
    }
    );


  int a;
  std::cin >> a;
	*//*
  return 0;
}




/*#include "database_mysql_real_connection.h"

#include "database_mysql_store_result.h"

#include <exception>


int main() {
  try {
  database::MySQLRealConnection mysqlRealConnection("localhost", "root", "root", "test", 4000);
  auto storeResult = mysqlRealConnection.retrieve("show tables");
  while(storeResult->mysqlFetchRow()) {
    do {
      std::cout << storeResult->getCurrentAttribute();
    } while(storeResult->setAtNextAttribute());
    std::cout << std::endl;
  }
  } catch(const std::exception&e) {
    std::cout << "greska: " <<  e.what() << std::endl;
  }
  return 0;
}*/
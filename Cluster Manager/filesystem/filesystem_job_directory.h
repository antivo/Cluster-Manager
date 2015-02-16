#ifndef FILESYSTEM_JOB_DIRECTORY
#define FILESYSTEM_JOB_DIRECTORY

#include <string>
#include <memory>
#include <vector>

namespace filesystem {
	class Directory;

  class JobDirectory {
  public:
		JobDirectory(const std::string& rootPath, const std::string& clientId, const std::string& jobName);
    ~JobDirectory();

		void prepareForDataIncome(const std::string& dataDirectory);
		void prepareForResultIncome(const std::string& resultDirectory);

		/// throw
    void storeData(const std::string& relativePath, const std::string& content) const; 
		Directory deployData(const Directory& dst, const int uuid) const;
		void fetchResult(Directory& src) const;
		void remove();

		std::string getResultFilePath(const int W) const;

		std::vector<std::string> getResultTopology() const;
		std::vector<std::string> getRelativeResultTopology() const;

	private:
		static const std::string deployedDirectory;
		static const std::string resultFile;

		std::unique_ptr<Directory> jobDirectory;
		std::unique_ptr<Directory> data;
		std::unique_ptr<Directory> result;

    JobDirectory(const JobDirectory&) = delete;
    JobDirectory& operator=(const JobDirectory&) = delete;
  };

}

#endif
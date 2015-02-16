#include "filesystem_directory.h"

#include <windows.h>

#include "filesystem_common.h"
#include "filesystem_directory_common.h"
#include "filesystem_file_common.h"

namespace filesystem {
	Directory::Directory(const std::string path) : path(path) {
		if (!directory::exists(path)) {
			directory::create(path);
		}
	}

	Directory::Directory(const Directory& other) {
		this->path = other.path;
	}

	Directory& Directory::operator=(const Directory& other) {
		this->path = other.path;
		return *this;
	}

	Directory::~Directory() {}

	Directory Directory::makeSubdirectory(const std::string& name) const {
		auto subDirectoryPath = filesystem::addToPath(path, name);
		return subDirectoryPath;
	}

	void Directory::move(const Directory& destination) {
		const auto found = path.find_last_of(R"(/\)");
		const auto filename = path.substr(found + 1);
		auto destinationDir = destination.makeSubdirectory(filename);
		filesystem::directory::move(path, destinationDir.path);
		this->path = destinationDir.path;
	}

	void filesystem::Directory::copyAndRemove(const Directory & destination) {
		const auto found = path.find_last_of(R"(/\)");
		const auto filename = path.substr(found + 1);
		auto destinationDir = destination.makeSubdirectory(filename);
		copy(destinationDir);
		remove();
	}

	void Directory::copy(const Directory & destination) const {
		filesystem::directory::copy(path, destination.path);
	}

	void Directory::remove() {
		filesystem::directory::remove(path.c_str());
		path.clear();
	}

	void Directory::store(const std::string& relativePath, const std::string& content) const {
		const auto found = relativePath.find_last_of(R"(/\)");
		std::string directory, filename;
		if (std::string::npos == found) {
			directory = path;
			filename = relativePath;
		} else {
			directory = filesystem::addToPath(path, relativePath.substr(0, found));
			filename = relativePath.substr(found + 1);
		}
		if (!directory::exists(directory)) {
			directory::create(directory);
		}
		const auto filePath = filesystem::addToPath(directory, filename);
		filesystem::file::create(filePath, content);
	}

	std::string filesystem::Directory::getPath() const {
		return path;
	}

	std::string Directory::appendToPath(const std::string& suffix) const {
		return filesystem::addToPath(path, suffix);
	}

	std::vector<std::string> Directory::createTopology() const {
		return filesystem::directory::getTopology(path);
	}
}
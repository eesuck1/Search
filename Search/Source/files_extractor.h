#ifndef FILES_EXTRACTOR_H
#define FILES_EXTRACTOR_H

#include <fstream>
#include <format>
#include <unordered_map>
#include <filesystem>
#include <future>

#include "constants.h"
#include "utilities.h"


class FilesExtractor
{
private:
	std::vector<std::future<void>> registration_futures;
	std::vector<std::future<void>> read_futures;
	std::mutex registration_mutex;
	std::mutex read_mutex;
public:
	std::vector<std::string> read_txt(std::string file_path);
	std::vector<std::string> read_doc(std::string file_path);
	std::vector<std::string> read_pdf(std::string file_path);
	std::vector<std::string> read_file(std::string file_path);
	std::vector<std::string> get_arguments(std::string file_path);
	std::string get_basename(std::string file_path);
	std::string get_extension(std::string file_path);
	std::unordered_map<std::string, size_t> register_files(std::string files_root);
	void read_file_async(std::unordered_map<std::string, std::vector<std::string>>& result,
		std::string file_path, size_t file_size);
	void register_file(std::unordered_map<std::string, size_t>& destination, std::filesystem::directory_entry file_entry);
	std::unordered_map<std::string, std::vector<std::string>> read_files(std::string files_root);
};

#endif // FILES_EXTRACTOR_H
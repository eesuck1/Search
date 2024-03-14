#include "files_extractor.h"


std::vector<std::string> FilesExtractor::get_arguments(std::string file_path)
{
	std::string file_name = FilesExtractor::get_basename(file_path) + ".txt";
	std::string destination = '"' + txt_folder_path + file_name + '"';

	std::vector<std::string> arguments = { '"' + file_path + '"', destination };

	return arguments;
}

std::string FilesExtractor::get_basename(std::string file_path)
{
	return std::filesystem::path(file_path).filename().stem().string();
}

std::string FilesExtractor::get_extension(std::string file_path)
{
	return std::filesystem::path(file_path).extension().string();
}

void FilesExtractor::register_file(std::unordered_map<std::string, size_t>& destination, std::filesystem::directory_entry file_entry)
{
	std::string file_path = file_entry.path().string();
	std::string extension = FilesExtractor::get_extension(file_path);

	if (extension != ".pdf" && extension != ".doc" && extension != ".docx" && extension != ".txt")
	{
		return;
	}

	std::lock_guard<std::mutex> lock(FilesExtractor::registration_mutex);
	destination.insert({ file_path, file_entry.file_size() });
}

std::unordered_map<std::string, size_t> FilesExtractor::register_files(std::string files_root)
{
	std::unordered_map<std::string, size_t> result;

	std::filesystem::path root = files_root;

	for (const auto& file_entry : std::filesystem::recursive_directory_iterator(root))
	{
		FilesExtractor::registration_futures.push_back(std::async(std::launch::async, [&] {
			FilesExtractor extractor;
			return extractor.register_file(result, file_entry);
			}));
	}

	return result;
}

std::vector<std::string> FilesExtractor::read_txt(std::string file_path)
{
	std::ifstream file;
	std::vector<std::string> result;
	std::filesystem::path path = file_path.substr(1, file_path.size() - 2);

	file.open(path);

	if (file.is_open())
	{
		std::string line;

		while (!file.eof())
		{
			file >> line;
			result.push_back(line);
		}
	
		file.close();
	}
	else
	{
		std::cout << "[ERROR] Could not read - " << path << '\n';
	}

	return result;
};

std::vector<std::string> FilesExtractor::read_doc(std::string file_path)
{
	std::vector<std::string> paths = FilesExtractor::get_arguments(file_path);
	std::string arguments = join(paths, " ");

	run_python(python_doc_parser, arguments);

	std::vector<std::string> result = FilesExtractor::read_txt(paths.back());

	return result;
}

std::vector<std::string> FilesExtractor::read_pdf(std::string file_path)
{
	std::vector<std::string> paths = FilesExtractor::get_arguments(file_path);
	std::string arguments = join(paths, " ");

	run_python(python_pdf_parser, arguments);

	std::vector<std::string> result = FilesExtractor::read_txt(paths.back());

	return result;
}

std::vector<std::string> FilesExtractor::read_file(std::string file_path)
{
	std::string extension = FilesExtractor::get_extension(file_path);

	if (extension == ".txt")
	{
		return FilesExtractor::read_txt(file_path);
	}

	if (extension == ".pdf")
	{
		return FilesExtractor::read_pdf(file_path);
	}

	if (extension == ".doc" || extension == ".docx")
	{
		return FilesExtractor::read_doc(file_path);
	}

	if (extension == "")
	{
		return std::vector<std::string>();
	}

	std::vector<std::string> result = { std::filesystem::path(file_path).filename().string()};

	return result;
}

void FilesExtractor::read_file_async(std::unordered_map<std::string, std::vector<std::string>>& result,
	std::string file_path, size_t file_size)
{
	std::lock_guard<std::mutex> lock(FilesExtractor::read_mutex);

	result.insert({ file_path , FilesExtractor::read_file(file_path) });
	std::cout << "[" << file_size << " B] " << file_path << " Readed\n";
}

std::unordered_map<std::string, std::vector<std::string>> FilesExtractor::read_files(std::string files_root)
{
	std::unordered_map<std::string, std::vector<std::string>> result;
	std::unordered_map<std::string, size_t> files_registration = FilesExtractor::register_files(files_root);

	for (const auto& registration : files_registration)
	{
		FilesExtractor::read_futures.push_back(
			std::async(std::launch::async, [&result, registration] {
				FilesExtractor extractor;
				extractor.read_file_async(
					result, registration.first, registration.second
				); })
		);
	}

	return result;
}

#include <iostream>

#include "files_extractor.h"


int main(void)
{
	FilesExtractor extractor;
	
	std::string root = "C:\\LPNU\\2\\SPZ\\Kursova\\Test\\";
	std::unordered_map<std::string, std::vector<std::string>> result = extractor.read_files(root);

	for (const auto& key : result)
	{
		std::cout << key.first << std::endl;
	}

	return 0;
}
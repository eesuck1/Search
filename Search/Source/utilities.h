#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <iostream>
#include <windows.h>

#include "constants.h"


template<typename T>
static void print_vector(std::vector<T> vector)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	for (T item : vector)
	{
		std::cout << item << std::endl;
	}
}

static std::string join(std::vector<std::string> strings, std::string delimiter)
{
	std::string result;

	result += strings.front();

	for (size_t index = 1; index < strings.size() - 1; index++)
	{
		result += delimiter + strings.at(index);
	}

	result += " " + strings.back();

	return result;
}

static void run_python(std::string script_path, std::string arguments)
{
	std::vector<std::string> paths = { executable_root_offset + python_interpreter , executable_root_offset + script_path, arguments };
	std::string command = join(paths, " ");

	system(command.c_str());
}

#endif // !UTILITIES_H

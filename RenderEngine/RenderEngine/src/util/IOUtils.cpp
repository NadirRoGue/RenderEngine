#include "util/IOUtils.h"

#include <fstream>

char * Engine::IO::loadStringFromFile(const char * fileName, unsigned long long & fileLen)
{
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file) return 0;

	file.seekg(0, std::ios::end);
	fileLen = file.tellg();
	file.seekg(std::ios::beg);

	char * content = new char[fileLen + 1];

	int i = 0;
	while (file.good())
	{
		content[i] = char(file.get());
		if (!file.eof()) i++;
		else fileLen = i;
	}

	content[fileLen] = '\0';
	file.close();

	return content;
}
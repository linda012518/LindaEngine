#include "TextLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace LindaEngine;

std::string TextLoader::Load(const char* path)
{
	std::string str;

	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);
		std::stringstream strStream;
		// read file's buffer contents into streams
		strStream << file.rdbuf();
		// close file handlers
		file.close();
		// convert stream into string
		return strStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << path << "ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return NULL;
	}

}


void TextLoader::DeleteAnnotation(std::string& tex)
{
	while (true)
	{
		size_t attrPos = tex.find("//");
		if (attrPos == std::string::npos)
			return;
		size_t pos = tex.find('\n', attrPos);
		if (pos == std::string::npos)
			tex.erase(attrPos);
		else
			tex.erase(attrPos, pos - attrPos);
	}
}

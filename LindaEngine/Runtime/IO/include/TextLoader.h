#pragma once

#include <string>
#include <vector>

namespace LindaEngine
{
	class TextLoader
	{
	public:
		static std::string Load(const char* path);
		static void DeleteAnnotation(std::string& tex);
		static std::vector<std::string> SplitString(std::string& tex, char sign);
	};
}

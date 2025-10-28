#pragma once

#include <string>

namespace LindaEngine
{
	class TextLoader
	{
	public:
		static std::string Load(const char* path);
		static void DeleteAnnotation(std::string& tex);
	};
}

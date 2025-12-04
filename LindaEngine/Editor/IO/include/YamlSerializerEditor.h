#pragma once

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>

namespace LindaEditor
{
	class YamlSerializerEditor
	{
	public:
		static std::vector<std::string> DeSerializeEditorPanel();
	};
}

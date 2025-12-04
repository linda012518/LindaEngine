#include "YamlSerializerEditor.h"

using namespace LindaEditor;

std::vector<std::string> YamlSerializerEditor::DeSerializeEditorPanel()
{
	std::vector<std::string> result;

	YAML::Node data;
	try
	{
		data = YAML::LoadFile("Built-inAssets/Config/EditorPanelConfig");
	}
	catch (const std::exception&)
	{
		return result;
	}

	data = data["EditorPanelConfig"];
	if (!data)
		return result;

	for (const auto& panel : data) {
		result.push_back(panel.as<std::string>());
	}

	return result;

}

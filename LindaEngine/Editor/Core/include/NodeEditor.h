#pragma once

#include <string>

namespace LindaEditor
{
	struct NodeEditor
	{
		virtual ~NodeEditor() = default;
		std::string path;
		std::string name;
	};

	struct SceneNodeEditor : public NodeEditor
	{
		int index = -1;
	};

	struct ShaderNodeEditor : public NodeEditor
	{

	};

	struct MaterialNodeEditor : public NodeEditor
	{

	};
}

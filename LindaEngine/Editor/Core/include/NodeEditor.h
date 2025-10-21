#pragma once

#include <string>

namespace LindaEditor
{
	struct NodeEditor
	{
		virtual ~NodeEditor() = default;
	};

	struct SceneNodeEditor : public NodeEditor
	{
		int index = -1;
		std::string path;
		std::string name;
	};
}

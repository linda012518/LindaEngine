#pragma once

#include "MaterialStateEditor.h"

#include <string>

namespace LindaEngine { class Scene; }

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
		Ref<LindaEngine::Scene> scene;
	};

	struct ShaderNodeEditor : public NodeEditor
	{

	};

	struct MaterialNodeEditor : public NodeEditor
	{

	};
}

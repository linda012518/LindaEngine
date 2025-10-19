#pragma once

namespace LindaEditor
{
	struct NodeEditor
	{
		virtual ~NodeEditor() = default;
	};

	struct SceneNodeEditor : public NodeEditor
	{
		int index = -1;
		const char* path = nullptr;
		const char* name = nullptr;
	};
}

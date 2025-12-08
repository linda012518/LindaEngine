#pragma once

#include "AutoPtr.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class Entity;
}

namespace LindaEditor
{
	struct SceneNodeEditor;

	class SceneManagerEditor
	{
	public:
		static bool SaveScene(std::string path = "");
		static void AddToBuild(int index, Ref<SceneNodeEditor> scene);
		static void RemoveToBuild(Ref<SceneNodeEditor> scene);
		static bool Build(const char* path);
		static bool LoadScene(std::string path = "");
		static bool IsNewScene();
		static Ref<SceneNodeEditor> GetCurrentNode();

		static int Initialize();

	private:
		static Ref<SceneNodeEditor> _node;
		static std::vector<Ref<SceneNodeEditor>> _buildScenes;
		static std::vector<Ref<SceneNodeEditor>> _sceneNodes;
		static std::string _defaultScenePath;
	};
}

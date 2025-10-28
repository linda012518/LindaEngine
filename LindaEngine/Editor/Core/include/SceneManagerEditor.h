#pragma once

#include "AutoPtr.h"

#include <vector>
#include <string>

namespace LindaEngine { class Scene; }

namespace LindaEditor
{
	struct SceneNodeEditor;

	class SceneManagerEditor
	{
	public:
		static bool SaveScene();
		static void AddToBuild(int index, Ref<SceneNodeEditor> scene);
		static void RemoveToBuild(Ref<SceneNodeEditor> scene);
		static bool Build(const char* path);
		static bool LoadScene();
		static Ref<SceneNodeEditor> GetCurrentNode();
		static Ref<LindaEngine::Scene> GetCurrentScene();

		static void Initialize();

	private:
		static Ref<LindaEngine::Scene> _scene;
		static Ref<SceneNodeEditor> _node;
		static std::vector<Ref<SceneNodeEditor>> _buildScenes;
		static std::vector<Ref<SceneNodeEditor>> _sceneNodes;
	};
}

#pragma once

#include "AutoPtr.h"

#include <unordered_map>
#include <string>

namespace LindaEngine
{
	class Scene;

	class SceneManager
	{
	public:
		static int Initialize();
		static void Finalize();
		static void Tick();

		static Ref<Scene> LoadScene(const char* name);
		static Ref<Scene> LoadScene(int index);
		static Ref<Scene> LoadSceneByPath(std::string path);

		static Ref<Scene> GetCurrentScene() { return _currentScene; }

	private:
		static Ref<Scene> LoadSceneFromPath(std::string& path);

	private:
		static bool _loadFinish;
		static Ref<Scene> _currentScene;
		static std::unordered_map<int, std::string> _sceneIndexMap;
		static std::unordered_map<std::string, std::string> _sceneNameMap;
	};
}

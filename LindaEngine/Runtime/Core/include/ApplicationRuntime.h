#pragma once

#include "Application.h"
#include "SceneManager.h"

namespace LindaEngine
{
	class ApplicationRuntime : public Application
	{
	public:
		int Initialize();
		void Finalize();
		void Tick();

	private:
		Scope<SceneManager> _sceneManager;
	};
}

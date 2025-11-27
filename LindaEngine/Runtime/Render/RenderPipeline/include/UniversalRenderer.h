#pragma once

#include "AutoPtr.h"
#include "ScriptablePass.h"

#include <vector>

namespace LindaEngine
{
	class Camera;

	class UniversalRenderer
	{
	public:
		void Initialize();
		void Finalize();
		void Render(Camera* camera);

	private:
		void AddRenderPass(Ref<ScriptablePass> pass);
		void CollectRenderPass();

	private:
		std::vector<Ref<ScriptablePass>> _renderPasses;
	};
}

#pragma once

#include "AutoPtr.h"
#include "ScriptablePass.h"

#include <vector>

namespace LindaEngine
{
	class Camera;

	class ScriptableRenderer
	{
	public:
		void Initialize();
		void Finalize();
		void Render(Camera* camera);

	protected:
		void AddRenderPass(Ref<ScriptablePass> pass);
		virtual void CollectRenderPass() = 0;

	protected:
		std::vector<Ref<ScriptablePass>> _renderPasses;
	};
}

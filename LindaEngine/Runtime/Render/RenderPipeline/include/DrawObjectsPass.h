#pragma once

#include "ScriptablePass.h"
#include "Settings.h"

namespace LindaEngine
{
	class DrawObjectsPass : public ScriptablePass
	{
	public:
		DrawObjectsPass(RenderPassEvent event, DrawingSettings& settings);

		void Render(Camera* camera);

	private:
		DrawingSettings _settings;
	};
}


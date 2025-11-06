#pragma once

#include "AutoPtr.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class Renderer;
	class Camera;
	struct DrawingSettings;

	class RendererSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Renderer* renderer);
		static void Remove(Renderer* renderer);
		static void Clear();

		static void DrawRenderers(Camera* camera, Ref<DrawingSettings> settings);
		static void Cull(Camera* camera, Ref<DrawingSettings> settings);

	private:
		static void FillDrawables(Ref<DrawingSettings> settings);
		static void SortDrawables(Ref<DrawingSettings> settings);

	private:
		static std::vector<Renderer*> _components;
		static std::vector<Renderer*> _renderables;
	};
}


#pragma once

#include "AutoPtr.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class Renderer;
	struct DrawingSettings;

	class RendererSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Renderer* renderer);
		static void Remove(Renderer* renderer);
		static void Clear();

		static void DrawRenderers(Ref<DrawingSettings> settings);
		static void Cull();

	private:
		static void FillDrawables(Ref<DrawingSettings> settings);
		static void SortDrawables(Ref<DrawingSettings> settings);

	private:
		static std::vector<Renderer*> _components;
		static std::vector<Renderer*> _culledRenderers;
		static std::vector<Renderer*> _renderables;
	};
}


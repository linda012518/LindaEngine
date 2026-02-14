#pragma once

#include "AutoPtr.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class Renderer;
	class Camera;
	class Material;
	struct DrawingSettings;
	struct Drawable;

	class RendererSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Renderer* renderer);
		static void Remove(Renderer* renderer);
		static void Clear();

		static void DrawRenderers(Camera* camera, DrawingSettings* settings);
		static void DrawRenderer(Renderer* renderer, Ref<Material> material);
		static void DrawSkybox();
		static void DrawAdjunct();
		static void DrawErrorRenderer();

	private:
		static void Cull(Camera* camera, DrawingSettings* settings);
		static void FillDrawables(DrawingSettings* settings);
		static void SortDrawables(DrawingSettings* settings);

	private:
		static std::vector<Renderer*> _components;
		static std::vector<Renderer*> _tempRenderables;
		static std::vector<Ref<Drawable>> _drawables;
	};
}


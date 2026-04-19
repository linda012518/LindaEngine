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
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();

		static void Add(Weak<Renderer> renderer);
		static void Remove(Weak<Renderer> renderer);
		static void Clear();

		static void DrawRenderers(Weak<Camera> camera, DrawingSettings& settings);
		static void DrawRenderer(Weak<Renderer> renderer, Ref<Material> material);
		static void DrawSkybox(Weak<Camera> camera);
		static void DrawAdjunct();
		static void DrawErrorRenderer();

		static std::vector<Weak<Renderer>>& GetRenderers() { return _components; }

	private:
		static void Cull(Weak<Camera> camera, DrawingSettings& settings);
		static void FillDrawables(DrawingSettings& settings);
		static void SortDrawables(DrawingSettings& settings);

	private:
		static std::vector<Weak<Renderer>> _components;
		static std::vector<Weak<Renderer>> _tempRenderables;
		static std::vector<Ref<Drawable>> _drawables;
	};
}


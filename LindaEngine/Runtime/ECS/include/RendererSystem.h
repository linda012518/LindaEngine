#pragma once

#include "AutoPtr.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Renderer;
	class MaterialPass;
	struct Drawable;

	class RendererSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Renderer* renderer);
		static void Remove(Renderer* renderer);
		static void Clear();

		static void DrawRenderers();

	private:
		static void AddDrawable(std::string lightMode, Ref<Drawable> da);
		static void RemoveDrawable(Renderer* renderer);
		static void CreateDrawable(Renderer* renderer, int materialIndex, std::string lightMode, MaterialPass* pass, int queue);

	private:
		static std::vector<Renderer*> _components;
		static std::unordered_map<std::string, std::vector<Ref<Drawable>>> _lightModeToDrawables;
	};
}


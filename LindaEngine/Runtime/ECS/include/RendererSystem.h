#pragma once

#include <vector>

namespace LindaEngine
{
	class Renderer;

	class RendererSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Renderer* renerer);
		static void Remove(Renderer* renerer);
		static void Clear();

	private:
		static std::vector<Renderer*> _components;
	};
}


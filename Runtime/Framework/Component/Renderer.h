#pragma once

#include "Core/Component.hpp"
#include <vector>

namespace LindaEngine
{
	class Mesh;
	class Material;

	class Renderer : public Component
	{
	public:
		Renderer();
		virtual ~Renderer();

		void SetMesh(Mesh* m);

		void AddMaterial(Material* mat);

		void Render();

	private:
		Mesh* _mesh;
		std::vector<Material*> _materials;
	};
}

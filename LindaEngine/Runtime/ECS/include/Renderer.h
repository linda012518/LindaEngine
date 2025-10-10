#pragma once

#include "AutoPtr.h"
#include "Component.h"

#include <vector>

namespace LindaEngine
{
	class Material;
	class Mesh;

	class Renderer : public Component
	{
	public:
		Renderer(Entity& entity, bool enable = true);
		virtual ~Renderer();

	protected:
		std::vector<Ref<Material>> _materialList;
		Ref<Mesh> _mesh;
	};

	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(Entity& entity, bool enable = true);
		virtual ~MeshRenderer();


	};

	class SkinMeshRenderer : public Renderer
	{
	public:
		SkinMeshRenderer(Entity& entity, bool enable = true);
		virtual ~SkinMeshRenderer();


	};
}

#pragma once

#include "AutoPtr.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

namespace LindaEngine
{
	struct Drawable
	{
		Ref<Material> material;
		Mesh::Data* meshData;
		Transform* transform;
		float distanceToCamera;

		void Draw();
	};
}
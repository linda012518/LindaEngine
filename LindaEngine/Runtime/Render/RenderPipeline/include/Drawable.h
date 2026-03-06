#pragma once

#include "AutoPtr.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

namespace LindaEngine
{
	struct Drawable
	{
		Ref<Material> material = nullptr;
		Mesh::Data* meshData = nullptr;
		Transform* transform = nullptr;
		float distanceToCamera = 0.0f;

		void Draw();
	};
}
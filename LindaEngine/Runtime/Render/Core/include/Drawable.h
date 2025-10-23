#pragma once

#include "glm/glm.hpp"
#include "Mesh.h"

namespace LindaEngine
{
	class MaterialPass;
	class Renderer;

	struct Drawable
	{
		Mesh::Data* mesh;
		MaterialPass* material;
		const glm::mat4* localToWorld;
		const glm::mat4* worldToLocal;
		int renderQueue;
		Renderer* renderer;
	};
}

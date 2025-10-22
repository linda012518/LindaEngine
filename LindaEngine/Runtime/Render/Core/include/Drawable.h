#pragma once

namespace LindaEngine
{
	class Mesh { public: struct Data; };
	class MaterialPass;
	class Transform;

	struct Drawable
	{
		Mesh::Data* mesh;
		MaterialPass* material;
		Transform* transform;
	};
}

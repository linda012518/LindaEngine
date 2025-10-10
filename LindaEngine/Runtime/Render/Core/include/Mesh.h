#pragma once

#include "LObject.h"

#include <vector>

namespace LindaEngine
{
	enum class DrawType
	{
		POINTS			= 0x0000,
		LINES			= 0x0001,
		LINE_LOOP		= 0x0002,
		LINE_STRIP		= 0x0003,
		TRIANGLES		= 0x0004,
		TRIANGLE_STRIP	= 0x0005,
		TRIANGLE_FAN	= 0x0006,
		QUADS			= 0x0007
	};

	enum class VertexDataType
	{
		None, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat2, Mat3, Mat4, Bool, Bool2, Bool3, Bool4
	};

	enum class VertexAttributeType
	{
		Position, Color, Normal, Tangent, UV0, UV1, UV2, UV3, UV4, UV5, UV6, UV7, BoneID1, BoneID2, BoneWeights1, BoneWeights2
	};

	struct VertexAttribute
	{
		VertexAttributeType attributeType;
		VertexDataType dateType;
		uint32_t size;
		uint32_t offset;
		uint32_t index;
		bool normalized;
	};

	class Mesh : public LObject
	{
	public:
		const int GetMeshCount() { return _meshCount; }

	private:
		int _meshCount = 1;
		std::vector<VertexAttribute> _attributes;
	};
}

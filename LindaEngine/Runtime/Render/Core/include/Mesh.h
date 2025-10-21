#pragma once

#include "LObject.h"
#include "glm/glm.hpp"
#include "ISerializable.h"

#include <unordered_map>
#include <string>
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
		None, Position, Normal, Tangent, UV0, UV1, UV2, UV3, UV4, UV5, UV6, UV7, Color, BoneID1, BoneID2, BoneWeights1, BoneWeights2
	};

	struct VertexAttribute
	{
		VertexAttribute(VertexAttributeType attrType, uint32_t offset_, uint32_t index_);

		std::string attributeName;
		VertexAttributeType attributeType;
		VertexDataType dateType;
		uint32_t size;
		uint32_t offset;
		uint32_t index;
		bool normalized;

		static const char* GetAttributeName(VertexAttributeType attrType);
		static VertexDataType GetAttributeDataType(VertexAttributeType attrType);
		static uint32_t GetAttributeDataSize(VertexAttributeType attrType);
		static VertexAttributeType GetAttributeType(const char* name);
	};

	class VertexArray;

	class Mesh : public LObject, public ISerializable
	{
	public:
		struct Data
		{
			unsigned int VAO = -1;
			unsigned int VBO = -1;
			unsigned int IBO = -1;

			std::vector<float> vertexData;
			std::vector<unsigned int> indexData;
			size_t indexSize = -1;
			int attributeIndex = 0;
			int attributeOffset = 0;
			std::vector<VertexAttribute> attributes;

			void AddAttribute(VertexAttributeType attrType);
			void AddAttribute(const char* name);
			VertexAttribute* GetAttribute(const char* name);
			VertexAttribute* GetAttribute(VertexAttributeType attrType);
		};
	public:
		Mesh::Data& AddMeshData(Mesh::Data data);
		const int GetMeshCount() { return _meshCount; }

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		int _meshCount = 0;
		std::vector<Mesh::Data> _data;
		std::string _path;
	};
}

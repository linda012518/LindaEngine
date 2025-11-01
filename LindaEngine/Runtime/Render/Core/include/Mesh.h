#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"
#include "RenderEnumData.h"
#include "glm/glm.hpp"

#include <unordered_map>
#include <string>
#include <vector>

namespace LindaEngine
{
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
		VertexAttribute(VertexAttributeType attrType);

		std::string attributeName;
		VertexAttributeType attributeType;
		VertexDataType dateType;
		uint32_t size;
		uint32_t offset;
		uint32_t index;
		bool normalized;

		static const char* GetAttributeName(VertexAttributeType attrType);
		static const char* GetVertexDataTypeName(VertexDataType type);
		static VertexDataType GetAttributeDataType(VertexAttributeType attrType);
		static uint32_t GetAttributeDataSize(VertexAttributeType attrType);
		static VertexAttributeType GetAttributeType(std::string& name);
	};

	class VertexArray;

	class Mesh : public LObject, public ISerializable
	{
	public:
		struct Data
		{
			Ref<VertexArray> vertexArray;

			std::vector<float> vertexData;
			std::vector<unsigned int> indexData;
			std::vector<VertexAttribute> attributes;

			int vertexStride = 0;
			DrawType drawType = DrawType::TRIANGLES;

			void AddAttribute(VertexAttributeType attrType);
			void AddAttribute(std::string name);
			void Draw();

		private:
			void CalculateStride();
		};
	public:
		Mesh::Data& AddMeshData(Mesh::Data data);
		Mesh::Data* GetMeshData(int index = 0) { return &_data[index]; }
		const std::vector<VertexAttribute>& GetMeshAttributes(int index = 0) { return _data[index].attributes; }
		const int GetMeshCount() { return _meshCount; }
		void SetPath(const char* path) { _path = path; }
		void Draw(int index);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		int _meshCount = 0;
		std::vector<Mesh::Data> _data;
		std::string _path;
	};
}

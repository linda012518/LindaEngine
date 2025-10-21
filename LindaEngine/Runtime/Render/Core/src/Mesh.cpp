#include "Mesh.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

VertexAttribute::VertexAttribute(VertexAttributeType attrType, uint32_t offset_, uint32_t index_)
{
	attributeName = GetAttributeName(attrType);
	attributeType = attrType;
	dateType = GetAttributeDataType(attrType);
	size = GetAttributeDataSize(attrType);
	offset = offset_;
	index = index_;
	normalized = false;
}

const char* VertexAttribute::GetAttributeName(VertexAttributeType attrType)
{
	switch (attrType)
	{
	case VertexAttributeType::Position:		return "aPosition";
	case VertexAttributeType::Normal:		return "aNormal";
	case VertexAttributeType::Tangent:		return "aTangent";
	case VertexAttributeType::UV0:			return "aUV0";
	case VertexAttributeType::UV1:			return "aUV1";
	case VertexAttributeType::UV2:			return "aUV2";
	case VertexAttributeType::UV3:			return "aUV3";
	case VertexAttributeType::UV4:			return "aUV4";
	case VertexAttributeType::UV5:			return "aUV5";
	case VertexAttributeType::UV6:			return "aUV6";
	case VertexAttributeType::UV7:			return "aUV7";
	case VertexAttributeType::Color:		return "aColor";
	case VertexAttributeType::BoneID1:		return "aBoneID1";
	case VertexAttributeType::BoneID2:		return "aBoneID2";
	case VertexAttributeType::BoneWeights1: return "aBoneWeights1";
	case VertexAttributeType::BoneWeights2: return "aBoneWeights2";
	}
	return "aNone";
}

VertexDataType VertexAttribute::GetAttributeDataType(VertexAttributeType attrType)
{
	switch (attrType)
	{
	case VertexAttributeType::Position:		return VertexDataType::Float3;
	case VertexAttributeType::Normal:		return VertexDataType::Float3;
	case VertexAttributeType::Tangent:		return VertexDataType::Float3;
	case VertexAttributeType::UV0:			return VertexDataType::Float2;
	case VertexAttributeType::UV1:			return VertexDataType::Float2;
	case VertexAttributeType::UV2:			return VertexDataType::Float2;
	case VertexAttributeType::UV3:			return VertexDataType::Float2;
	case VertexAttributeType::UV4:			return VertexDataType::Float2;
	case VertexAttributeType::UV5:			return VertexDataType::Float2;
	case VertexAttributeType::UV6:			return VertexDataType::Float2;
	case VertexAttributeType::UV7:			return VertexDataType::Float2;
	case VertexAttributeType::Color:		return VertexDataType::Float4;
	case VertexAttributeType::BoneID1:		return VertexDataType::Int4;
	case VertexAttributeType::BoneID2:		return VertexDataType::Int4;
	case VertexAttributeType::BoneWeights1: return VertexDataType::Float4;
	case VertexAttributeType::BoneWeights2: return VertexDataType::Float4;
	}
	return VertexDataType::None;
}

uint32_t VertexAttribute::GetAttributeDataSize(VertexAttributeType attrType)
{
	switch (attrType)
	{
	case VertexAttributeType::Position:		return 4 * 3;
	case VertexAttributeType::Normal:		return 4 * 3;
	case VertexAttributeType::Tangent:		return 4 * 3;
	case VertexAttributeType::UV0:			return 4 * 2;
	case VertexAttributeType::UV1:			return 4 * 2;
	case VertexAttributeType::UV2:			return 4 * 2;
	case VertexAttributeType::UV3:			return 4 * 2;
	case VertexAttributeType::UV4:			return 4 * 2;
	case VertexAttributeType::UV5:			return 4 * 2;
	case VertexAttributeType::UV6:			return 4 * 2;
	case VertexAttributeType::UV7:			return 4 * 2;
	case VertexAttributeType::Color:		return 4 * 4;
	case VertexAttributeType::BoneID1:		return 4 * 4;
	case VertexAttributeType::BoneID2:		return 4 * 4;
	case VertexAttributeType::BoneWeights1: return 4 * 4;
	case VertexAttributeType::BoneWeights2: return 4 * 4;
	}
	return 0;
}

VertexAttributeType VertexAttribute::GetAttributeType(std::string& name)
{
	if (name == "aPosition")			return VertexAttributeType::Position;
	else if (name == "aNormal")			return VertexAttributeType::Normal;
	else if (name == "aTangent")		return VertexAttributeType::Tangent;
	else if (name == "aUV0")			return VertexAttributeType::UV0;
	else if (name == "aUV1")			return VertexAttributeType::UV1;
	else if (name == "aUV2")			return VertexAttributeType::UV2;
	else if (name == "aUV3")			return VertexAttributeType::UV3;
	else if (name == "aUV4")			return VertexAttributeType::UV4;
	else if (name == "aUV5")			return VertexAttributeType::UV5;
	else if (name == "aUV6")			return VertexAttributeType::UV6;
	else if (name == "aUV7")			return VertexAttributeType::UV7;
	else if (name == "aColor")			return  VertexAttributeType::Color;
	else if (name == "aBoneID1")		return VertexAttributeType::BoneID1;
	else if (name == "aBoneID2")		return  VertexAttributeType::BoneID2;
	else if (name == "aBoneWeights1")	return VertexAttributeType::BoneWeights1;
	else if (name == "aBoneWeights2")	return VertexAttributeType::BoneWeights2;
	else return VertexAttributeType::None;
}

Mesh::Data& Mesh::AddMeshData(Mesh::Data data)
{
	_data.push_back(data);
	_meshCount++;
	return _data[_data.size() - 1];
}

void Mesh::Data::AddAttribute(VertexAttributeType attrType)
{
	attributes.push_back(VertexAttribute(attrType, attributeOffset, attributeIndex));
	VertexAttribute* attr = GetAttribute(attrType);
	attributeOffset += attr->size;
	attributeIndex++;
}

void Mesh::Data::AddAttribute(std::string name)
{
	VertexAttributeType attrType = VertexAttribute::GetAttributeType(name);
	attributes.push_back(VertexAttribute(attrType, attributeOffset, attributeIndex));
	VertexAttribute* attr = GetAttribute(attrType);
	attributeOffset += attr->size;
	attributeIndex++;
}

VertexAttribute* Mesh::Data::GetAttribute(const char* name)
{
	for (auto& attr : attributes) {
		if (attr.attributeName != name)
			continue;
		return &attr;
	}
	return nullptr;
}

VertexAttribute* Mesh::Data::GetAttribute(VertexAttributeType attrType)
{
	for (auto& attr : attributes) {
		if (attr.attributeType != attrType)
			continue;
		return &attr;
	}
	return nullptr;
}

bool Mesh::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "FilePath" << YAML::Value << _path;
	out << YAML::EndMap;

	return true;
}

bool Mesh::Deserialize(YAML::Node& node)
{
	_path = node["FilePath"].as<std::string>();
	return true;
}
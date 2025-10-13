#pragma once

#include <yaml-cpp/yaml.h>

namespace LindaEngine
{
	class YamlSerializer
	{
	public:
		static void Serialize(const char* path);
		static bool DeSerialize(const char* path);

		static void SerializeMaterial(const char* path);
		static bool DeSerializeMaterial(const char* path);

		static void SerializeGraphicsConfig(const char* path);
		static bool DeSerializeGraphicsConfig(const char* path);

	private:
		static void SerializeMaterialPass(YAML::Emitter& out);
	};
}

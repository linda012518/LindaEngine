#pragma once

#include <yaml-cpp/yaml.h>
#include "AutoPtr.h"

namespace LindaEngine
{
	class Material;
	struct Texture;
	class YamlSerializer
	{
	public:
		static void SerializeMaterial(const char* path);
		static Ref<Material> DeSerializeMaterial(const char* path);

		static void SerializeTexture(const char* path);
		static Ref<Texture> DeSerializeTexture(const char* path);

		static void SerializeGraphicsConfig(const char* path);
		static bool DeSerializeGraphicsConfig(const char* path);

	public:
		static YAML::Emitter* out;

	private:
		static void SerializeMaterialPass(YAML::Emitter& out);
		static void SerializeMaterialUniform(YAML::Emitter& out, void* uniform);
	};
}

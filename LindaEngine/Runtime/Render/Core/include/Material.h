#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"
#include "MaterialState.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Transform;
	class MaterialPass;
	struct VertexAttribute;

	class Material : public LObject, public ISerializable
	{
		friend class YamlSerializer;
		friend class RendererSystem;
		friend class MaterialManager;

	public:
		Material();
		virtual ~Material();

		void SetPath(const char* path) { _state.materialPath = path; }
		void Bind(Ref<MaterialPass> pass, Transform* transform, const std::vector<VertexAttribute>& attributes);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		std::vector<Ref<MaterialPass>> GetPassByLightMode(std::string& lightMode);

	private:
		MaterialState _state;
		//std::unordered_map<std::string, Ref<MaterialPass>> _passes;
		std::vector<Ref<MaterialPass>> _passes;

	public:
		static Ref<Material> overrideMat;
		static std::string overrideLightMode;
	};
}

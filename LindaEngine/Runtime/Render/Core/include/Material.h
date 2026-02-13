#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"
#include "MaterialState.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Transform;
	class MaterialPass;
	struct VertexAttribute;
	struct Texture;

	class Material : public LObject, public ISerializable
	{
		friend class YamlSerializer;
		friend class RendererSystem;
		friend class MaterialManager;

	public:
		Material();
		virtual ~Material();

		void SetTexture(const std::string& name, std::string value, int pass = 0);
		void SetTexture(const std::string& name, Ref<Texture> value, int pass = 0, int renderTextureColorIndex = 0);//renderTextureColorIndex = -1 »°…Ó∂»
		void SetInt(const std::string& name, int value, int pass = 0);
		void SetFloat(const std::string& name, float value, int pass = 0);
		void SetIntArray(const std::string& name, int count, int* value, int pass = 0);
		void SetFloatArray(const std::string& name, int count, float* value, int pass = 0);

		void SetIVec4(const std::string& name, const glm::ivec4& value, int count = 1, int pass = 0);
		void SetVec4(const std::string& name, const glm::vec4& value, int count = 1, int pass = 0);
		void SetMat2(const std::string& name, const glm::mat2& mat, int count = 1, int pass = 0);
		void SetMat3(const std::string& name, const glm::mat3& mat, int count = 1, int pass = 0);
		void SetMat4(const std::string& name, const glm::mat4& mat, int count = 1, int pass = 0);

		void SetPath(const char* path) { _state.materialPath = path; }
		std::string& GetPath() { return _state.materialPath; }
		void Bind(Ref<MaterialPass> pass, Transform* transform, const std::vector<VertexAttribute>& attributes);
		void Bind(int pass, Transform* transform, const std::vector<VertexAttribute>& attributes);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		bool CanRender(std::string& lightMode, int minQueue, int maxQueue);
		bool HasLightMode(std::string& lightMode);
		std::vector<Ref<MaterialPass>> GetPassByLightMode(std::string& lightMode);
		int GetRenderQueue() { return _state.renderQueue; }

	private:
		MaterialState _state;
		//std::unordered_map<std::string, Ref<MaterialPass>> _passes;
		std::vector<Ref<MaterialPass>> _passes;

	public:
		static Ref<Material> overrideMat;
		static std::string overrideLightMode;
		static bool isPostProcess;
		static bool isPickPass;
	};
}

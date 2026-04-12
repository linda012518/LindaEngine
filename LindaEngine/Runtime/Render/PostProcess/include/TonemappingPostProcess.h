#pragma once

#include "PostProcessEffectRenderer.h"
#include "ClassDeclare.inl"

namespace LindaEngine
{
	class Material;

	class TonemappingPostProcess : public PostProcessEffectRenderer
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(TonemappingPostProcess)

		TonemappingPostProcess();
		~TonemappingPostProcess();

		void Initialize();
		void Finalize();
		void Render(Ref<RenderTexture> src, Ref<RenderTexture> dest);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		Ref<Material> _simpleTonemapping;

	};
}

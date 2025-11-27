#pragma once

#include "PostProcessEffectRenderer.h"
#include "ClassDeclare.inl"

namespace LindaEngine
{
	class Material;

	class GrayPostProcess : public PostProcessEffectRenderer
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(GrayPostProcess)

		GrayPostProcess();
		~GrayPostProcess();

		void Initialize();
		void Finalize();
		void Render(Ref<RenderTexture> src, Ref<RenderTexture> dest);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		Ref<Material> _grayMaterial;
	};
}


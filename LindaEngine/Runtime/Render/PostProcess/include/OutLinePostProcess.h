#pragma once

#include "PostProcessEffectRenderer.h"
#include "ClassDeclare.inl"

namespace LindaEngine
{
	class Material;

	class OutLinePostProcess : public PostProcessEffectRenderer
	{
	public:
		DECLARE_DYNAMIC_CREATE_CLASS(OutLinePostProcess)

		OutLinePostProcess();
		~OutLinePostProcess();

		void Initialize();
		void Finalize();
		void Render(Ref<RenderTexture> src, Ref<RenderTexture> dest);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		Ref<Material> _maskMaterial;
		Ref<Material> _maskMaterialSkin;
		Ref<Material> _edgeDetectionMaterial;
		Ref<Material> _blurMaterial;
		Ref<Material> _overlayMaterial;
		
	};
}

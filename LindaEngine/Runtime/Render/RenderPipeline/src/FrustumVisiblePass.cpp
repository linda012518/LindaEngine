#include "FrustumVisiblePass.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Drawable.h"
#include "FBXManager.h"
#include "ShaderBuiltInUniform.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "Light.h"
#include "LightSystem.h"
#include "RenderPipelineEditor.h"

using namespace LindaEngine;
using namespace LindaEditor;

FrustumVisiblePass::FrustumVisiblePass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
	_material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/FrustumVisible.shader");
	_defaultLineMaterial = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/BoundingBox.shader");
}

void FrustumVisiblePass::Render(Camera* camera)
{
	std::string temp = Material::overrideLightMode;
	Material::overrideLightMode = "Adjunct";

	const std::vector<Camera*> cameraList = CameraSystem::GetActiveCameraList();
	for (auto& playCamera : cameraList)
	{
		_material->SetUniformValue<glm::mat4>(ShaderBuiltInUniform::linda_FrustumMatrix_I_VP.c_str(), playCamera->GetViewProjectInverseMatrix());
		Drawable drawable;
		drawable.material = _material;
		drawable.meshData = FBXManager::GetFrustumMesh()->GetMeshData();
		drawable.Draw();
	}

	const std::vector<Light*> lights = LightSystem::GetLightList(RenderPipelineEditor::activeCamera);
	for (auto& light : lights)
	{
		if (light->GetLightType() == LightType::SpotLight)
			continue;
		Drawable drawable;
		drawable.material = _defaultLineMaterial;
		drawable.meshData = FBXManager::GetLightMesh(light->GetLightType())->GetMeshData();
		drawable.transform = light->GetTransform();
		drawable.Draw();
	}

	Material::overrideLightMode = temp;
}
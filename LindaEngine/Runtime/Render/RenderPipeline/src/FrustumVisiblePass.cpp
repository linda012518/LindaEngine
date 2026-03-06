#include "FrustumVisiblePass.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Drawable.h"
#include "FBXManager.h"
#include "ShaderBuiltInUniform.h"
#include "Camera.h"
#include "CameraSystem.h"

using namespace LindaEngine;

FrustumVisiblePass::FrustumVisiblePass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
	_material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/FrustumVisible.shader");
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
	Material::overrideLightMode = temp;
}
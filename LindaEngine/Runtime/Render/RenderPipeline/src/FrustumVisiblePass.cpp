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
	_cameraMaterial = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/FrustumVisible.shader");
	_lightMaterial = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/LightVisible.shader");
}

void FrustumVisiblePass::Render(Camera* camera)
{
	std::string temp = Material::overrideLightMode;
	Material::overrideLightMode = "Adjunct";

	const std::vector<Camera*> cameraList = CameraSystem::GetActiveCameraList();
	for (auto& playCamera : cameraList)
	{
		_cameraMaterial->SetUniformValue<glm::mat4>(ShaderBuiltInUniform::linda_FrustumMatrix_I_VP.c_str(), playCamera->GetViewProjectInverseMatrix());
		Drawable drawable;
		drawable.material = _cameraMaterial;
		drawable.meshData = FBXManager::GetFrustumMesh()->GetMeshData();
		drawable.Draw();
	}

	const std::vector<Light*> lights = LightSystem::GetLightList(RenderPipelineEditor::activeCamera);
	for (auto& light : lights)
	{
		glm::mat4 model = glm::mat4(1.0f);
		switch (light->GetLightType())
		{
		case LightType::DirectionLight:
		{
			glm::vec3 position = light->GetTransform()->GetWorldPosition();
			glm::quat rotation = light->GetTransform()->GetWorldRotation();

			model = glm::mat4_cast(rotation);

			model[3][0] = position.x;
			model[3][1] = position.y;
			model[3][2] = position.z;
			model[3][3] = 1.0f;

			_lightMaterial->SetUniformValue<glm::vec4>("color", glm::vec4(0.85, 0.85, 0.4, 1.0));
			DrawLightWireframe(LightType::DirectionLight, model);
		}
		break;
		case LightType::SpotLight:
		{
			_lightMaterial->SetUniformValue<glm::vec4>("color", glm::vec4(0.85, 0.85, 0.4, 1.0));
			CalculateSpotLightMat(model, light, true);
			DrawLightWireframe(LightType::SpotLight, model);
			_lightMaterial->SetUniformValue<glm::vec4>("color", glm::vec4(0.35, 0.35, 0.2, 1.0));
			CalculateSpotLightMat(model, light, false);
			DrawLightWireframe(LightType::SpotLight, model);
		}
		break;
		case LightType::PointLight:
		{
			glm::vec3 position = light->GetTransform()->GetWorldPosition();
			float scale = ((PointLight*)light)->GetRange();

			model[0][0] = scale;
			model[1][1] = scale;
			model[2][2] = scale;

			model[3][0] = position.x;
			model[3][1] = position.y;
			model[3][2] = position.z;
			model[3][3] = 1.0f;

			_lightMaterial->SetUniformValue<glm::vec4>("color", glm::vec4(0.85, 0.85, 0.4, 1.0));
			DrawLightWireframe(LightType::PointLight, model);
		}
		break;
		}
	}

	Material::overrideLightMode = temp;
}

void FrustumVisiblePass::DrawLightWireframe(LightType type, glm::mat4& model)
{
	_lightMaterial->SetUniformValue<glm::mat4>(ShaderBuiltInUniform::linda_LightLocalToWorld.c_str(), model);
	Drawable drawable;
	drawable.material = _lightMaterial;
	drawable.meshData = FBXManager::GetLightMesh(type)->GetMeshData();
	drawable.Draw();
}

void FrustumVisiblePass::CalculateSpotLightMat(glm::mat4& model, Light* light, bool isInner)
{
	glm::vec3 position = light->GetTransform()->GetWorldPosition();
	glm::quat rotation = light->GetTransform()->GetWorldRotation();
	SpotLight* spotLight = ((SpotLight*)light);
	float angle = isInner ? spotLight->GetInnerAngle() : spotLight->GetOuterAngle();
	float distance = spotLight->GetRange();
	float scaleXY = glm::tan(glm::radians(angle / 2)) * distance;  // 根据距离和角度计算底面半径
	float scaleZ = distance;							  // 轴向长度

	model[0][0] = scaleXY;
	model[1][1] = scaleXY;
	model[2][2] = scaleZ;

	model = glm::mat4_cast(rotation) * model;

	model[3][0] = position.x;
	model[3][1] = position.y;
	model[3][2] = position.z;
	model[3][3] = 1.0f;

	//scaleXY = tan(outerAngle / 2) * distance  // 根据距离和角度计算底面半径
	//scaleZ = distance                        // 轴向长度
	//S = glm::scale(glm::mat4(1.0), glm::vec3(scaleXY, scaleXY, scaleZ))
	//R = 旋转矩阵，将局部Z轴(0, 0, 1)旋转到光的方向
	//T = glm::translate(glm::mat4(1.0), lightPosition)
	//modelMatrix = T * R * S

	//// 假设已经计算好 radius, range, dir, pos
	//glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, range));
	//glm::mat4 R = glm::orientation(glm::normalize(dir), glm::vec3(0, 0, 1)); // 使Z轴指向dir
	//glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
	//glm::mat4 model = T * R * S;
}

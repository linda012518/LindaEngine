#include "RendererSystem.h"
#include "Renderer.h"
#include "Settings.h"
#include "Material.h"
#include "Entity.h"
#include "Camera.h"
#include "Transform.h"
#include "CullTool.h"
#include "Drawable.h"
#include "MaterialManager.h"
#include "ShaderBuiltInUniform.h"

#include <algorithm>

using namespace LindaEngine;

std::vector<Renderer*> RendererSystem::_components;
std::vector<Renderer*> RendererSystem::_tempRenderables;
std::vector<Ref<Drawable>> RendererSystem::_drawables;

void RendererSystem::Tick(float deltaTime)
{
	for (auto& com : _components) {
		if (false == com->IsEnable())
			continue;
		com->Tick();
	}
}

void RendererSystem::OnDeserializeFinish()
{
	//for (auto& com : _components) {

	//}
}

void RendererSystem::Add(Renderer* renderer)
{
	_components.push_back(renderer);
}

void RendererSystem::Remove(Renderer* renderer)
{
	auto itr = std::find(_components.begin(), _components.end(), renderer);
	if (itr != _components.end())
		_components.erase(itr);
}

void RendererSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "RendererSystem is not empty, Check destruction process.");

	_drawables.clear();
	_tempRenderables.clear();

	std::vector<Renderer*> temp;

	for (auto& com : _components) {
		if (false == com->GetEntity().GetDontDestory())
			continue;
		temp.push_back(com);
	}

	_components.clear();

	for (auto& com : temp) {
		_components.push_back(com);
	}
}

void RendererSystem::DrawRenderers(Camera* camera, DrawingSettings* settings)
{
	Material::overrideLightMode = settings->lightMode;

	Cull(camera, settings);
	FillDrawables(settings);
	SortDrawables(settings);

	for (auto& drawable : _drawables)
	{
		drawable->Draw();
	}
}

void RendererSystem::DrawRenderer(Renderer* renderer, Ref<Material> material)
{
	int count = (int)renderer->GetMesh()->GetAllMeshData().size();
	for (int i = 0; i < count; i++)
	{
		Drawable da;
		da.material = material;
		da.meshData = renderer->GetMesh()->GetMeshData(i);
		da.transform = renderer->GetTransform();
		da.Draw();
	}
}

void RendererSystem::DrawSkybox()
{
	Renderer::RenderSkybox();
}

void RendererSystem::DrawAdjunct()
{
	for (auto& com : _components) {
		com->RenderBoundingBox();
	}
}

void RendererSystem::DrawErrorRenderer()
{
	static bool isLoaded = false;
	static Ref<Material> errorDefault = nullptr;
	static Ref<Material> errorSkin = nullptr;
	static Drawable drawable;
	if (isLoaded == false)
	{
		errorDefault = MaterialManager::GetDefaultMaterial("BuiltInAssets/Shaders/Error.shader");
		errorSkin = MaterialManager::GetDefaultMaterial("BuiltInAssets/Shaders/Error.shader", true);
		isLoaded = true;
	}

	for (auto& renderer : _components)
	{
		int count = (int)renderer->GetMesh()->GetAllMeshData().size();
		for (int i = 0; i < count; i++)
		{
			if (false == renderer->HasError(i))
				continue;

			SkinMeshRenderer* skinPtr = dynamic_cast<SkinMeshRenderer*>(renderer);
			if (nullptr != skinPtr)
			{
				std::vector<glm::mat4>& matrices = skinPtr->GetFinalBoneMatrix();
				errorSkin->SetUniformValue<glm::mat4*>(ShaderBuiltInUniform::linda_BonesMatrices.c_str(), matrices.data(), (int)matrices.size());
				drawable.material = errorSkin;
			}
			else
			{
				drawable.material = errorDefault;
			}

			drawable.meshData = renderer->GetMesh()->GetMeshData(i);
			drawable.transform = renderer->GetTransform();
			drawable.Draw();
		}
	}
}

void RendererSystem::Cull(Camera* camera, DrawingSettings* settings)
{
	//TODO 距离剔除 视锥裁剪 遮挡剔除 可见灯光 可见反射Cubemap 可见SH
	Frustum& frustum = camera->GetFrustum();
	glm::vec3& cameraPos = (glm::vec3&)camera->GetTransform()->GetWorldPosition();
	float far = camera->GetFar();
	int layer = camera->GetLayerMask();
	glm::vec3 forward, up, right;
	camera->GetTransform()->GetWorldDir(forward, up, right);
	CullSettings& cs = settings->cullSettings;

	_tempRenderables.clear();

	for (auto& renderer : _components)
	{
		if (renderer->InLayerMask(layer) == false || renderer->IsSkybox() || renderer->IsEnable() == false)
			continue;

		AABBBoundingBox& box = renderer->GetBoundingBox();

		bool fresult = cs.frustumCull ? CullTool::FrustumCull(frustum, box) : true;
		bool oresult = cs.occlusionCull ? CullTool::OcclusionCull() : true;
		bool dresult = cs.distanceCull ? CullTool::DistanceCull(cameraPos, box, far) : true;

		if (fresult && oresult && dresult)
		{
			_tempRenderables.push_back(renderer);
			renderer->SetDistanceToCamera(glm::dot(cameraPos - renderer->GetTransform()->GetWorldPosition(), forward));
		}
	}
}

void RendererSystem::FillDrawables(DrawingSettings* settings)
{
	_drawables.clear();

	for (auto& renderer : _tempRenderables)
	{
		int count = (int)renderer->GetMesh()->GetAllMeshData().size();
		for (int i = 0; i < count; i++)
		{
			if (false == renderer->CanRender(i, settings->renderQueueRange.minQueue, settings->renderQueueRange.maxQueue))
				continue;

			Ref<Drawable> da = renderer->GetDrawable(i);
			da->distanceToCamera = renderer->GetDistanceToCamera();
			_drawables.push_back(da);
		}
	}
}

void RendererSystem::SortDrawables(DrawingSettings* settings)
{
	switch (settings->sortSettings.criteria)
	{
	case SortingCriteria::SortingLayer:
		break;
	case SortingCriteria::RenderQueue:
	{
		std::sort(_drawables.begin(), _drawables.end(), [](Ref<Drawable> a, Ref<Drawable> b) {
				return a->material->GetRenderQueue() < b->material->GetRenderQueue(); });
	}
	break;
	case SortingCriteria::FarToNear:
	{
		std::sort(_drawables.begin(), _drawables.end(), [](Ref<Drawable> a, Ref<Drawable> b) {
			return a->distanceToCamera > b->distanceToCamera; });
	}
	break;
	case SortingCriteria::NearToFar:
	{
		std::sort(_drawables.begin(), _drawables.end(), [](Ref<Drawable> a, Ref<Drawable> b) {
			return a->distanceToCamera < b->distanceToCamera; });
	}
	break;
	case SortingCriteria::OptimizeStateChanges:
		break;
	case SortingCriteria::CanvasOrder:
		break;
	case SortingCriteria::RendererPriority:
		break;
	case SortingCriteria::CommonOpaque:
	{
		std::sort(_drawables.begin(), _drawables.end(), [](Ref<Drawable> a, Ref<Drawable> b) { 
			if (a->material->GetRenderQueue() != b->material->GetRenderQueue()) {
				return a->material->GetRenderQueue() < b->material->GetRenderQueue();
			}
			return a->distanceToCamera < b->distanceToCamera;
		});
	}
	break;
	case SortingCriteria::CommonTransparent:
	{
		std::sort(_drawables.begin(), _drawables.end(), [](Ref<Drawable> a, Ref<Drawable> b) {
			if (a->material->GetRenderQueue() != b->material->GetRenderQueue()) {
				return a->material->GetRenderQueue() < b->material->GetRenderQueue();
			}
			return a->distanceToCamera > b->distanceToCamera;
			});
	}
	break;
	}
}


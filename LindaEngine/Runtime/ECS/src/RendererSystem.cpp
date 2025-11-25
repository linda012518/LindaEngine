#include "RendererSystem.h"
#include "Renderer.h"
#include "Settings.h"
#include "Material.h"
#include "Entity.h"
#include "Camera.h"
#include "Transform.h"
#include "CullTool.h"
#include "Drawable.h"

#include <algorithm>

using namespace LindaEngine;

std::vector<Renderer*> RendererSystem::_components;
std::vector<Renderer*> RendererSystem::_tempRenderables;
std::vector<Ref<Drawable>> RendererSystem::_drawables;

void RendererSystem::Tick()
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
	_components.clear();
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

void RendererSystem::DrawSkybox()
{
	Renderer::RenderSkybox();
}

void RendererSystem::Cull(Camera* camera, DrawingSettings* settings)
{
	//TODO 距离剔除 视锥裁剪 遮挡剔除 可见灯光 可见反射Cubemap 可见SH
	Frustum& frustum = camera->GetFrustum();
	glm::vec3& cameraPos = (glm::vec3&)camera->GetTransform()->GetWorldPosition();
	float far = camera->GetFar();
	glm::vec3 forward, up, right;
	camera->GetTransform()->GetWorldDir(forward, up, right);
	CullSettings& cs = settings->cullSettings;

	_tempRenderables.clear();

	for (auto& renderer : _components)
	{
		if (renderer->IsEnable() == false)
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
			if (false == renderer->CanRender(i, settings->layerMask, settings->renderQueueRange.minQueue, settings->renderQueueRange.maxQueue))
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
		break;
	case SortingCriteria::FarToNear:
		break;
	case SortingCriteria::NearToFar:
		break;
	case SortingCriteria::OptimizeStateChanges:
		break;
	case SortingCriteria::CanvasOrder:
		break;
	case SortingCriteria::RendererPriority:
		break;
	case SortingCriteria::CommonOpaque:
	{
		//std::sort(_renderables.begin(), _renderables.end(), [](Renderer* a, Renderer* b) { return a->GetDistanceToCamera() < b->GetDistanceToCamera(); });

	}
	break;
	case SortingCriteria::CommonTransparent:
		break;
	}
}


#include "RendererSystem.h"
#include "Renderer.h"
#include "Settings.h"
#include "Material.h"
#include "Entity.h"

using namespace LindaEngine;

std::vector<Renderer*> RendererSystem::_components;
std::vector<Renderer*> RendererSystem::_culledRenderers;
std::vector<Renderer*> RendererSystem::_renderables;

void RendererSystem::Tick()
{
	for (auto& com : _components) {
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

	_components.clear();
	_culledRenderers.clear();
	_renderables.clear();
}

void RendererSystem::DrawRenderers(Ref<DrawingSettings> settings)
{
	FillDrawables(settings);
	SortDrawables(settings);

	std::vector<std::string>& lightModes = settings->lightModes;

	for (auto& renderer : _components)
	{
		for (auto& lightMode : lightModes)
		{
			Material::overrideLightMode = lightMode;
			renderer->Render(renderer->GetEntity().GetTransform());
		}
	}
}

void RendererSystem::Cull()
{
	_culledRenderers.clear();
	_renderables.clear();
	//TODO 视锥裁剪 遮挡剔除 可见灯光 可见反射Cubemap 可见SH
}

void RendererSystem::FillDrawables(Ref<DrawingSettings> settings)
{
	//TODO Check Layer、Queue、LightMode
}

void RendererSystem::SortDrawables(Ref<DrawingSettings> settings)
{
	//TODO
}


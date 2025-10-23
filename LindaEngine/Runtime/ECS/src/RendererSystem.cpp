#include "RendererSystem.h"
#include "Renderer.h"
#include "Material.h"
#include "MaterialPass.h"
#include "Drawable.h"
#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"

using namespace LindaEngine;

std::vector<Renderer*> RendererSystem::_components;
std::vector<Renderer*> RendererSystem::_culledRenderers;
std::vector<Ref<Drawable>> RendererSystem::_drawables;

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
	_drawables.clear();
}

void RendererSystem::DrawRenderers(Ref<DrawingSettings> settings)
{
	FillDrawables(settings);
	SortDrawables(settings);
	for (auto& drawable : _drawables) {
		//Ref<Material> material = com->GetMaterials()[0];
		//material->CompileShader();
		//Ref<MaterialPass> pass = material->GetColorPass()[0];
		//pass->Bind(com->GetEntity().GetTransform()->GetLocalToWorldMat());
		//com->GetMesh()[0];
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void RendererSystem::Cull()
{
	_culledRenderers.clear();
	_drawables.clear();
	//TODO 视锥裁剪 遮挡剔除 可见灯光 可见反射Cubemap 可见SH
}

void RendererSystem::FillDrawables(Ref<DrawingSettings> settings)
{
	//TODO 
}

void RendererSystem::SortDrawables(Ref<DrawingSettings> settings)
{
	//TODO
}


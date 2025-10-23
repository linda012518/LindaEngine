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
std::unordered_map<std::string, std::vector<Ref<Drawable>>> RendererSystem::_lightModeToDrawables;

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
	std::vector<Ref<Material>>& materials = renderer->GetMaterials();
	int index = 0;
	for (auto mat : materials)
	{
		if (mat->_renderType == RenderType::Opaque)
		{
			Ref<MaterialPass> pass = nullptr;
			if (nullptr != mat->_depthNormalPass || (nullptr == mat->_depthNormalPass && mat->_hasFallback))
			{
				pass = nullptr == mat->_depthNormalPass ? Material::defaultDepthNormalPass : mat->_depthNormalPass;
				CreateDrawable(renderer, index, "DepthNormal", pass.get(), mat->_renderQueue);
			}

			if (nullptr != mat->_depthPass || (nullptr == mat->_depthPass && mat->_hasFallback))
			{
				pass = nullptr == mat->_depthPass ? Material::defaultDepthPass : mat->_depthPass;
				CreateDrawable(renderer, index, "Depth", pass.get(), mat->_renderQueue);
			}

			if (nullptr != mat->_shadowCasterPass || (nullptr == mat->_shadowCasterPass && mat->_hasFallback))
			{
				pass = nullptr == mat->_shadowCasterPass ? Material::defaultShadowCasterPass : mat->_shadowCasterPass;
				CreateDrawable(renderer, index, "ShadowCaster", pass.get(), mat->_renderQueue);
			}
		}

		for (auto colorPass : mat->_colorPasses)
		{
			CreateDrawable(renderer, index, colorPass->GetLightMode(), colorPass.get(), mat->_renderQueue);
		}

		index++;
	}
}

void RendererSystem::Remove(Renderer* renderer)
{
	auto itr = std::find(_components.begin(), _components.end(), renderer);
	if (itr != _components.end())
	{
		RemoveDrawable(renderer);
		_components.erase(itr);
	}
}

void RendererSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "RendererSystem is not empty, Check destruction process.");

	_components.clear();
}

void RendererSystem::DrawRenderers()
{
	//TODO 按RenderPipeline传过来Pass配置渲染
	for (auto& com : _components) {
		Ref<Material> material = com->GetMaterials()[0];
		material->CompileShader();
		Ref<MaterialPass> pass = material->GetColorPass()[0];
		pass->Bind(com->GetEntity().GetTransform()->GetLocalToWorldMat());
		//com->GetMesh()[0];
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void RendererSystem::AddDrawable(std::string lightMode, Ref<Drawable> da)
{
	if (_lightModeToDrawables.find(lightMode) != _lightModeToDrawables.end())
	{
		_lightModeToDrawables[lightMode].push_back(da);
	}
	else
	{
		_lightModeToDrawables[lightMode] = { da };
	}
}

void RendererSystem::RemoveDrawable(Renderer* renderer)
{
	for (auto& keyValue : _lightModeToDrawables)
	{
		auto& das = keyValue.second;
		for (auto itr = das.begin(); itr != das.end(); ++itr)
		{
			if ((*itr)->renderer != renderer)
				continue;
			das.erase(itr);
			break;
		}
	}
}

void RendererSystem::CreateDrawable(Renderer* renderer, int materialIndex, std::string lightMode, MaterialPass* pass, int queue)
{
	Transform* transform = renderer->GetEntity().GetTransform();
	Ref<Drawable> da = CreateRef<Drawable>();
	da->material = pass;
	da->mesh = renderer->GetMesh()->GetMeshData(materialIndex);
	da->localToWorld = &transform->GetLocalToWorldMat();
	da->worldToLocal = &transform->GetWorldToLocalMat();
	da->renderer = renderer;
	da->renderQueue = queue;
	AddDrawable(lightMode, da);
}


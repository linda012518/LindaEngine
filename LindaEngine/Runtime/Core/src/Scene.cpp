#include "Scene.h"
#include "Entity.h"
#include "Transform.h"

using namespace LindaEngine;

Entity* Scene::CreateEntity(const char* name, bool active)
{
	Ref<Entity> e = CreateRef<Entity>(name, active);
	_entitys.push_back(e);
	return e.get();
}

void Scene::DestroyEntity(Entity* entity)
{
	Transform* trans = entity->GetComponent<Transform>();
	trans->SetParent(nullptr);
	DestroyEntityIncludeChild(entity);
}

void Scene::DestroyEntityIncludeChild(Entity* entity)
{
	Transform* trans = entity->GetComponent<Transform>();
	const std::list<Transform*>& children = trans->GetChildren();
	for (auto& t : children)
	{
		DestroyEntityIncludeChild(&(t->GetEntity()));
	}

	for (auto iter = _entitys.begin(); iter != _entitys.end(); ++iter) {
		if ((*iter)->GetID() != entity->GetID())
			continue;

		entity->Destroy();
		_entitys.erase(iter);
		break;
	}
}

#include "Entity.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "UUID.h"
#include "ComponentFactory.h"
#include "Behavior.h"
#include "BehaviorSystem.h"
#include "Application.h"
#include "GUILayoutEditor.h"

#include "imgui/imgui.h"
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;
using namespace LindaEditor;

int Entity::_id = 0;

Entity::Entity(const char* name, bool active)
{
	_id++;
	_entityID = _id;

	_layer = 0xFFFFFFFF;
	_name = name;
	_active = active;
	_activeDirty = false;
	_uuid = UUID::Get();
	_isDontDestory = false;
	_hierarchyIndex = 0;

	Ref<Transform> c = CreateRef<Transform>(*this);
	_components.push_back(c);
	OnComponentAdded(c.get());
	_transform = c.get();
}

Entity::~Entity()
{
	Destroy();
}

void Entity::SetName(const std::string& name)
{
	_name = name;
}

const std::string& Entity::GetName() const
{
	return _name;
}

void Entity::SetActive(bool active)
{
	if (_active == active)
		return;

	bool activeLast = IsActive();
	_active = active;

	if (activeLast)
	{
		_activeDirty = true;
		UpdateChildrenDirty(_transform);
	}
}

bool Entity::IsActive()
{
	const Transform* parent = _transform->GetParent();
	return _active && (parent != nullptr ? parent->GetEntity().IsActive() : true);
}

void Entity::Destroy()
{
	for (auto& com : _components) {
		com->Destroy();
		OnComponentRemoved(com.get());
	}
	_components.clear();
	_behaviors.clear();
	_dirtyComponents.clear();
	_transform = nullptr;
}

void Entity::TransformDirty()
{
	for (auto& com : _components) {
		com->TransformDirty();
	}
}

Transform* Entity::GetTransform()
{
	return _transform;
}

void Entity::OnComponentAdded(Component* com)
{
	Behavior* pointer = dynamic_cast<Behavior*>(com);
	if (nullptr == pointer)
		return;

	if (Application::state != AppState::Running)
		BehaviorSystem::Add(pointer);
	else
	{
		_behaviors.push_back(pointer);
	}
}

void Entity::OnComponentRemoved(Component* com)
{
	Behavior* pointer = dynamic_cast<Behavior*>(com);
	if (nullptr != pointer)
	{
		pointer->OnDestroy();
		BehaviorSystem::Remove(pointer);
		return;
	}
}

void Entity::UpdateChildrenDirty(Transform* parent)
{
	for (auto& child : parent->GetChildren())
	{
		child->GetEntity()._activeDirty = true;
		UpdateChildrenDirty(child);
	}
}

void Entity::RemoveDirtyComponents()
{
	if (_dirtyComponents.empty())
		return;

	for (auto& com : _dirtyComponents)
	{
		for (auto it = _components.begin(); it != _components.end(); ++it)
		{
			if (com != (*it).get())
				continue;
			OnComponentRemoved(com);
			_components.erase(it);
			break;
		}
	}
	_dirtyComponents.clear();
}

void Entity::AddBehaviorsToSystem()
{
	if (_behaviors.empty())
		return;
	for (auto& b : _behaviors)
	{
		BehaviorSystem::Add(b);
	}
	_behaviors.clear();
}

bool Entity::IsDirty()
{
	return _activeDirty;
}

void Entity::ClearDirty()
{
	_activeDirty = false;
}

Component* Entity::AddComponent(std::string className)
{
	Ref<Component> pointer = ComponentFactory::CreateComponent(className, *this, true);
	_components.push_back(pointer);
	OnComponentAdded(pointer.get());
	return pointer.get();
}

void Entity::RemoveComponent(Component* com)
{
	static_assert(!std::is_same<Transform, Component>::value, "You can't remove a Transform from an actor");

	_dirtyComponents.push_back(com);
}

void Entity::RemoveComponentImmediately(Component* com)
{
	for (auto it = _components.begin(); it != _components.end(); ++it)
	{
		if (com != (*it).get())
			continue;
		OnComponentRemoved(com);
		_components.erase(it);
		break;
	}
}

bool Entity::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Name" << YAML::Value << _name;
	out << YAML::Key << "Layer" << YAML::Value << _layer;
	out << YAML::Key << "ID" << YAML::Value << _uuid;
	out << YAML::Key << "Active" << YAML::Value << _active;
	if (_isDontDestory)
		out << YAML::Key << "IsDontDestory" << YAML::Value << _isDontDestory;
	
	out << YAML::Key << "Components";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& com : _components)
	{
		com->Serialize();
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	return true;
}

bool Entity::Deserialize(YAML::Node& node)
{
	_uuid = node["ID"].as<std::string>();
	_layer = node["Layer"].as<int>();

	auto sign = node["IsDontDestory"];
	if (sign)
		_isDontDestory = node["IsDontDestory"].as<bool>();

	auto components = node["Components"];
	if (!components)
		return true;

	for (auto com : components)
	{
		std::string comName = com["Name"].as<std::string>();
		
		if (comName == "Transform")
		{
			_transform->Deserialize(com);
		}
		else
		{
			//Ref<Component> pointer = ComponentFactory::CreateComponent(comName, *this, com["enable"].as<bool>());
			//pointer->Deserialize(com);
			//_components.push_back(pointer);
			//OnComponentAdded(pointer.get());
			Component* pointer = AddComponent(comName);
			pointer->Deserialize(com);
		}
	}

	return true;
}

void Entity::OnImguiRender()
{
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	bool ret = IsActiveSelf();
	ImGui::Checkbox(std::string("##" + _name).c_str(), &ret);
	if (IsActiveSelf() != ret)
		SetActive(ret);

	ImGui::SameLine();
	std::string go = GetName();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18.0f);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##Name", &go);
	if (go != _name)
		SetName(go);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	std::vector<Ref<Component>>& coms = _components;
	for (auto& go : coms)
	{
		if (nullptr == go)
			continue;
		Component* component = go.get();
		bool isTransform = nullptr != dynamic_cast<Transform*>(component);

		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 0);

		ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 1.00f));
		if (isTransform)
			ImGui::BeginDisabled();

		std::string goID = "##" + std::string((const char*)component);
		bool ret = component->GetEnable();
		ImGui::Checkbox(goID.c_str(), &ret);
		if (component->GetEnable() != ret)
			component->SetEnable(ret);

		ImGui::PopStyleVar(1);
		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 1);

		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();

		if (isTransform)
			ImGui::EndDisabled();
		ImGui::PopStyleColor(1);

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.26f, 0.26f, 0.26f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.36f, 0.36f, 0.36f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.56f, 0.56f, 0.56f, 1.00f));

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (ImGui::TreeNodeEx((void*)component, treeNodeFlags, component->GetClassName().c_str()))
		{
			if (ImGui::IsItemHovered() && false == isTransform)
				_eidtorDirty = component;
			component->OnImguiRender();
			ImGui::TreePop();
		}

		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
		ImGui::PopStyleColor(1);
	}

	GUILayoutEditor::PopupContextMenu(
		[&]() {
			if (ImGui::MenuItem("Remove component"))
			{
				RemoveComponentImmediately(_eidtorDirty);
				_eidtorDirty = nullptr;
			}
		}, 
		[&]() {
			_eidtorDirty = nullptr;
		});
}

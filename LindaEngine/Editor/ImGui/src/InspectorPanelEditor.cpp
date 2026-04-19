#include "InspectorPanelEditor.h"
#include "EventSystemEditor.h"
#include "Entity.h"
#include "EventEditor.h"
#include "EventCodeEditor.h"
#include "glm/glm.hpp"
#include "ComponentFactory.h"
#include "GUILayoutEditor.h"
#include "Texture.h"
#include "Material.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(InspectorPanelEditor, ImGuiPanelEditor)

InspectorPanelEditor::InspectorPanelEditor()
{
}

void InspectorPanelEditor::OnImGuiRender()
{
	Initialize();
	bool close = true;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("  Inspector  ", &close, window_flags);

	if (nullptr == _object)
	{
		ImGui::End();
		return;
	}

	if (nullptr != _object)
	{
		Weak<Texture> texture = DynamicCastWeak(Texture, _object);
		Weak<Material> material = DynamicCastWeak(Material, _object);
		if (nullptr == texture && nullptr == material)
			_object->OnImguiRender();
		else
		{
			if (nullptr != material)
				Material::OnImguiRender(material);
			else
			{
				Weak<RenderTexture> rt = DynamicCastWeak(RenderTexture, _object);
				if (nullptr == rt)
					Texture::OnImguiRender(texture);
				else
					RenderTexture::OnImguiRender(rt);
			}
		}
	}

	DrawSundry();

	ImGui::End();
}

void InspectorPanelEditor::OnEvent(Weak<LindaEngine::IEventHandler> sender, int eventCode, Event& eventData)
{
	switch (eventCode)
	{
	case EventCodeEditor::SwitchSelectEntity:
	{
		SwitchSelectEntityEditor& event = dynamic_cast<SwitchSelectEntityEditor&>(eventData);
		_object = event.selectionEntity;
	}
	break;
	case EventCodeEditor::SwitchInspectorObject:
	{
		SwitchInspectorObjectEditor& event = dynamic_cast<SwitchInspectorObjectEditor&>(eventData);
		_object = event.lobject;
	}
	break;
	}
}

void InspectorPanelEditor::Initialize()
{
	static bool initialized = false;
	if (initialized)
		return;
	initialized = true;
	EventSystemEditor::Bind(EventCodeEditor::SwitchSelectEntity, shared_from_this());
	EventSystemEditor::Bind(EventCodeEditor::SwitchInspectorObject, shared_from_this());
}

void InspectorPanelEditor::DrawSundry()
{
	if (nullptr == _object)
		return;

	Weak<Entity> entity = DynamicCastWeak(Entity, _object);
	if (nullptr == entity)
		return;

	std::vector<std::string>& names = ComponentFactory::GetComponents();

	GUILayoutEditor::Dropdown("Add Component", names, [&](int index) {
		entity->AddComponent(names[index]);
		});
}


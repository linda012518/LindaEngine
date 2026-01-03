#include "PlayViewPanelEditor.h"
#include "TextureManager.h"
#include "Event.h"
#include "EventCode.h"
#include "EventSystem.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(PlayViewPanelEditor, ImGuiPanelEditor)

PlayViewPanelEditor::PlayViewPanelEditor()
{
	FramebufferTextureSpecification color;
	color.colorFormat = TextureFormat::RGBA8;
	_fboSpecs.push_back(color);
	FramebufferTextureSpecification depth;
	depth.colorFormat = TextureFormat::Depth16;
	depth.filter = FilterMode::Point;
	depth.isRenderBuffer = true;
	_fboSpecs.push_back(depth);

	_renderTexture = RenderTextureManager::Get(1, 1, _fboSpecs);
}

void PlayViewPanelEditor::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::Begin("Play View");

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0 && (_renderTexture->width != viewportPanelSize.x || _renderTexture->height != viewportPanelSize.y))
	{
		RenderTextureManager::DeleteImmediately(_renderTexture);
		RenderTextureManager::ClearLinkScreen();
		_renderTexture = RenderTextureManager::Get((int)viewportPanelSize.x, (int)viewportPanelSize.y, _fboSpecs);
	}

	WindowResizeEvent event;
	event.width = (int)viewportPanelSize.x;
	event.height = (int)viewportPanelSize.y;
	EventSystem::Dispatch(nullptr, EventCode::WindowResize, event);

	uint64_t textureID = _renderTexture->nativeIDs[0];
	ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	ImGui::PopStyleVar();
}

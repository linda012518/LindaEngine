#include "EditViewPanelEditor.h"
#include "TextureManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(EditViewPanelEditor, ImGuiPanelEditor)

EditViewPanelEditor::EditViewPanelEditor()
{
	//_viewportSize = glm::vec2(1.0f);
	_fboSpec.colorFormat = TextureFormat::RGBA8;
	_renderTexture = RenderTextureManager::Get(1, 1, _fboSpec);
}

void EditViewPanelEditor::OnImGuiRender()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 18));
	ImGui::Begin("Edit View");
	//ImGui::PopStyleVar();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	uint64_t textureID = _renderTexture->nativeIDs[0];
	ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

	if (_renderTexture->width != viewportPanelSize.x || _renderTexture->height != viewportPanelSize.y)
	{
		RenderTextureManager::DeleteImmediately(_renderTexture);
		_renderTexture = RenderTextureManager::Get((int)viewportPanelSize.x, (int)viewportPanelSize.y, _fboSpec);
	}

	ImGui::End();
}

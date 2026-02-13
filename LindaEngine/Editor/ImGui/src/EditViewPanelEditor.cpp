#include "EditViewPanelEditor.h"
#include "TextureManager.h"
#include "Event.h"
#include "EventCode.h"
#include "EventSystem.h"
#include "Graphic.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

bool EditViewPanelEditor::hovered = false;
Ref<RenderTexture> EditViewPanelEditor::pickRT = nullptr;

DYNAMIC_CREATE_CLASS(EditViewPanelEditor, ImGuiPanelEditor)

EditViewPanelEditor::EditViewPanelEditor()
{
	FramebufferTextureSpecification color;
	color.colorFormat = TextureFormat::RGBA8;
	_fboSpecs.push_back(color);
	FramebufferTextureSpecification color2;
	color2.colorFormat = TextureFormat::R32I;
	_fboSpecs.push_back(color2);
	FramebufferTextureSpecification depth;
	depth.colorFormat = TextureFormat::Depth16;
	depth.filter = FilterMode::Point;
	depth.isRenderBuffer = true;
	_fboSpecs.push_back(depth);

	_renderTexture = RenderTextureManager::Get(1, 1, _fboSpecs);

	_pickSpec.colorFormat = TextureFormat::R32I;
	pickRT = RenderTextureManager::Get(1, 1, _pickSpec);
}

void EditViewPanelEditor::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::Begin("Edit View");

	hovered = ImGui::IsWindowHovered();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0 && (_renderTexture->width != viewportPanelSize.x || _renderTexture->height != viewportPanelSize.y))
	{
		RenderTextureManager::DeleteImmediately(_renderTexture);
		RenderTextureManager::DeleteImmediately(pickRT);
		_renderTexture = RenderTextureManager::Get((int)viewportPanelSize.x, (int)viewportPanelSize.y, _fboSpecs);
		pickRT = RenderTextureManager::Get((int)viewportPanelSize.x, (int)viewportPanelSize.y, _pickSpec);
		RenderTextureManager::ClearLinkScreen();
	}

	uint64_t textureID = _renderTexture->nativeIDs[0];
	ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

	if (false == hovered)
	{
		_isLeftPressed = false;
		_isLeftFirst = true;
	}

	if (hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		if (_isLeftFirst)
		{
			_isLeftFirst = false;
			_isLeftPressed = true;

			ImVec2 mousePos = ImGui::GetMousePos();
			ImVec2 windowPos = ImGui::GetWindowPos();
			_firstPos = ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);
		}
	}
	if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		_isLeftPressed = false;
		_isLeftFirst = true;

		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 relativePos = ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);
		int xStart = _firstPos.x < relativePos.x ? _firstPos.x : relativePos.x;
		int yStart = _firstPos.y < relativePos.y ? _firstPos.y : relativePos.y;
		int xEnd = _firstPos.x > relativePos.x ? _firstPos.x : relativePos.x;
		int yEnd = _firstPos.y > relativePos.y ? _firstPos.y : relativePos.y;
		int width = xEnd - xStart;
		int height = yEnd - yStart;
		width = width > 0 ? width : 1;
		height = height > 0 ? height : 1;

		//int* data = (int*)Graphic::ReadPixed(pickRT, xStart, yStart, width, height);
		//for (int n = 0; n < width * height; n++)
		//{
		//	printf("%d\n", data[n]);
		//}
		//delete[] data;
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

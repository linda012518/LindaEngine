#include "EditViewPanelEditor.h"
#include "TextureManager.h"
#include "Graphic.h"
#include "EventCodeEditor.h"
#include "EventEditor.h"
#include "EventSystemEditor.h"
#include "CameraController.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <unordered_map>
#include <iostream>

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
	color2.filter = FilterMode::Point;
	_fboSpecs.push_back(color2);
	FramebufferTextureSpecification depth;
	depth.colorFormat = TextureFormat::Depth16;
	depth.filter = FilterMode::Point;
	depth.isRenderBuffer = true;
	_fboSpecs.push_back(depth);

	_renderTexture = RenderTextureManager::Get(1, 1, _fboSpecs);

	_pickSpec.colorFormat = TextureFormat::R32I;
	_pickSpec.filter = FilterMode::Point;
	pickRT = RenderTextureManager::Get(1, 1, _pickSpec);
}

void EditViewPanelEditor::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::Begin("Edit View");

	hovered = ImGui::IsWindowHovered();

	if (false == hovered)
	{
		_isLeftPressed = false;
		_isLeftFirst = true;
	}

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImVec2 windowPos = ImGui::GetCursorScreenPos();

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

	DrawRect(windowPos);
	ProcessPick(viewportPanelSize, windowPos);

	ImGui::End();
	ImGui::PopStyleVar();
}

void EditViewPanelEditor::ProcessPick(ImVec2& viewportPanelSize, ImVec2& windowPos)
{
	if (hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		if (_isLeftFirst)
		{
			_isLeftFirst = false;
			_isLeftPressed = true;

			ImVec2 mousePos = ImGui::GetMousePos();
			_firstPos = glm::ivec2(glm::abs(mousePos.x - windowPos.x), glm::abs(mousePos.y - windowPos.y));
		}
	}
	if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		_isLeftPressed = false;
		_isLeftFirst = true;

		ImVec2 mousePos = ImGui::GetMousePos();
		glm::ivec2 relativePos = glm::ivec2(glm::abs(mousePos.x - windowPos.x), glm::abs(mousePos.y - windowPos.y));
		int xStart = _firstPos.x < relativePos.x ? _firstPos.x : relativePos.x;
		int yStart = _firstPos.y < relativePos.y ? _firstPos.y : relativePos.y;
		int xEnd = _firstPos.x > relativePos.x ? _firstPos.x : relativePos.x;
		int yEnd = _firstPos.y > relativePos.y ? _firstPos.y : relativePos.y;
		int width = xEnd - xStart;
		int height = yEnd - yStart;
		width = width > 0 ? width : 1;
		height = height > 0 ? height : 1;
		int flippedYStart = (int)viewportPanelSize.y - yEnd - 1;

		std::unordered_map<int, int> selectID;
		int* data = (int*)Graphic::ReadPixed(pickRT, xStart, flippedYStart, width, height);
		for (int n = 0; n < width * height; n++)
		{
			int go = data[n];
			if (selectID.find(go) != selectID.end())
				continue;
			selectID[go] = go;
		}
		delete[] data;

		PickEntityIDEditor pick;
		pick.selectID = selectID;
		EventSystemEditor::Dispatch(nullptr, EventCodeEditor::PickEntityID, pick);
	}

}

void EditViewPanelEditor::DrawRect(ImVec2& windowPos)
{
	if (false == hovered || false == _isLeftPressed || true == CameraController::isLookRound)
		return;

	ImU32 fillColor = IM_COL32(0, 120, 255, 60);      // 半透明蓝色
	ImU32 borderColor = IM_COL32(255, 255, 255, 200); // 白色边框
	float borderThickness = 1.5f;
	float rounding = 0.0f;  // 圆角半径
	bool _showSizeText = true;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 currentMousePos = ImGui::GetMousePos();
	ImVec2 rectMin = ImVec2(windowPos.x + _firstPos.x, windowPos.y + _firstPos.y);
	ImVec2 rectMax = ImVec2(windowPos.x + (currentMousePos.x - windowPos.x), windowPos.y + (currentMousePos.y - windowPos.y));
	if (rectMin.x > rectMax.x) std::swap(rectMin.x, rectMax.x);
	if (rectMin.y > rectMax.y) std::swap(rectMin.y, rectMax.y);

	// 使用样式绘制
	draw_list->AddRectFilled(rectMin, rectMax, fillColor, rounding);
	draw_list->AddRect(rectMin, rectMax, borderColor, rounding, 0, borderThickness);

	if (_showSizeText)
	{
		char sizeText[32];
		snprintf(sizeText, sizeof(sizeText), "%.0f x %.0f", rectMax.x - rectMin.x, rectMax.y - rectMin.y);

		draw_list->AddText(
			ImVec2(currentMousePos.x + 15, currentMousePos.y - 25),
			IM_COL32(255, 255, 255, 255), sizeText);
	}
}

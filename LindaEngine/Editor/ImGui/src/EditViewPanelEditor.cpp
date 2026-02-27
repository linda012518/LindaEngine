#include "EditViewPanelEditor.h"
#include "TextureManager.h"
#include "Graphic.h"
#include "EventCodeEditor.h"
#include "EventEditor.h"
#include "EventSystemEditor.h"
#include "CameraController.h"
#include "RenderPipelineEditor.h"
#include "Camera.h"
#include "Entity.h"
#include "Transform.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/euler_angles.hpp"

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
	EventSystemEditor::Bind(EventCodeEditor::SwitchSelectEntity, this);

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

	RenderGuizmoButton(viewportPanelSize);
	RenderGuizmo();
	DrawRect(windowPos);
	ProcessPick(viewportPanelSize, windowPos);

	ImGui::End();
	ImGui::PopStyleVar();
}

void EditViewPanelEditor::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	SwitchSelectEntityEditor& event = dynamic_cast<SwitchSelectEntityEditor&>(eventData);
	_selectionEntity = event.selectionEntity;
}

void EditViewPanelEditor::ProcessPick(ImVec2& viewportPanelSize, ImVec2& windowPos)
{
	if (ImGuizmo::IsOver())
		return;
	if (true == CameraController::isLookRound)
	{
		_isLeftPressed = false;
		_isLeftFirst = true;
		return;
	}

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
	if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && _isLeftFirst == false)
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
	if (ImGuizmo::IsOver() || false == hovered || false == _isLeftPressed || true == CameraController::isLookRound)
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

void EditViewPanelEditor::RenderGuizmo()
{
	if (nullptr == _selectionEntity)
		return;

	glm::vec2 viewportBounds[2];
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);

	const glm::mat4& cameraProjection = RenderPipelineEditor::activeCamera->GetProjectMatrix();
	const glm::mat4& cameraView = RenderPipelineEditor::activeCamera->GetViewMatrix();
	glm::mat4 transform = _selectionEntity->GetTransform()->GetLocalToWorldMat();

	float snapValue = 0.01f; // Snap to 0.01m for translation/scale
	if (_gizmoType == ImGuizmo::OPERATION::ROTATE)
		snapValue = 1.0f;

	float snapValues[3] = { snapValue, snapValue, snapValue };

	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
		_gizmoType, _gizmoMode, glm::value_ptr(transform),
		nullptr, _gizmoSnap ? snapValues : nullptr);

	if (ImGuizmo::IsUsing())
	{
		_isLeftPressed = false;

		glm::vec3 translation, scale;
		glm::quat rotation;
		glm::vec3 skew;
		glm::vec4 m;
		glm::decompose(transform, scale, rotation, translation, skew, m);

		glm::quat origin = _selectionEntity->GetTransform()->GetWorldRotation();
		glm::quat deltaRotation = rotation - origin;
		_selectionEntity->GetTransform()->SetWorldPosition(translation);
		_selectionEntity->GetTransform()->SetWorldScale(scale);
		_selectionEntity->GetTransform()->SetWorldRotation(origin + deltaRotation);
	}

}

void EditViewPanelEditor::RenderGuizmoButton(ImVec2& viewportPanelSize)
{
	// 如果Edit View窗口太小，不渲染Gizmo面板
	float minWidth = 700;  // 最小宽度阈值
	float minHeight = 50;  // 最小高度阈值

	if (viewportPanelSize.x < minWidth || viewportPanelSize.y < minHeight)
		return;

	// 获取Edit View窗口的位置
	ImVec2 editViewPos = ImGui::GetWindowPos();

	// 设置Gizmo窗口的位置在Edit View的左上角
	ImGui::SetNextWindowPos(ImVec2(editViewPos.x + 10, editViewPos.y + 50), ImGuiCond_Always);

	// 创建Gizmo窗口，使用适当的窗口标志
	ImGuiWindowFlags gizmoWindowFlags =
		ImGuiWindowFlags_NoMove |           // 不可移动
		ImGuiWindowFlags_NoTitleBar |        // 无标题栏
		ImGuiWindowFlags_NoResize |          // 不可调整大小
		ImGuiWindowFlags_NoScrollbar |		// 无滚动条
		ImGuiWindowFlags_NoCollapse |         // 不可折叠
		ImGuiWindowFlags_AlwaysAutoResize;

	// 设置一个半透明的深色背景
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 0.9f)); // 半透明深色背景
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8)); // 增加内边距让按钮更好看
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4)); // 按钮内边距
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4)); // 项目间距

	// 固定窗口大小，确保所有按钮都能显示
	ImGui::SetNextWindowSize(ImVec2(minWidth, minHeight), ImGuiCond_Always);

	// 开始Gizmo窗口
	ImGui::Begin("Gizmo Controls", nullptr, gizmoWindowFlags);

	// 原有的Gizmo控件代码
	ImGui::Checkbox("Snap", &_gizmoSnap);
	ImGui::SameLine();
	ImGui::Text(" | ");
	ImGui::SameLine();
	if (ImGui::RadioButton("Translate", _gizmoType == ImGuizmo::TRANSLATE))
		_gizmoType = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", _gizmoType == ImGuizmo::ROTATE))
		_gizmoType = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", _gizmoType == ImGuizmo::SCALE))
		_gizmoType = ImGuizmo::SCALE;
	ImGui::SameLine();
	ImGui::Text(" | ");
	ImGui::SameLine();
	if (ImGui::RadioButton("Local", _gizmoMode == ImGuizmo::LOCAL))
		_gizmoMode = ImGuizmo::LOCAL;
	ImGui::SameLine();
	if (ImGui::RadioButton("World", _gizmoMode == ImGuizmo::WORLD))
		_gizmoMode = ImGuizmo::WORLD;

	ImGui::End();

	// 恢复样式
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor();
}

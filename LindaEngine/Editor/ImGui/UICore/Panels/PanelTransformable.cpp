#include "PanelTransformable.h"

#include "imgui/imgui.h"

using namespace LindaEditor;

PanelTransformable::PanelTransformable(const glm::vec2& p_defaultPosition, const glm::vec2& p_defaultSize, HorizontalAlignment p_defaultHorizontalAlignment, VerticalAlignment p_defaultVerticalAlignment, bool p_ignoreConfigFile) 
	: _defaultPosition(p_defaultPosition), _defaultSize(p_defaultSize), _defaultHorizontalAlignment(p_defaultHorizontalAlignment), _defaultVerticalAlignment(p_defaultVerticalAlignment), _ignoreConfigFile(p_ignoreConfigFile)
{
}

void PanelTransformable::SetPosition(const glm::vec2& p_position)
{
	_position = p_position;
	_positionChanged = true;
}

void PanelTransformable::SetSize(const glm::vec2& p_size)
{
	_size = p_size;
	_sizeChanged = true;
}

void PanelTransformable::SetAlignment(HorizontalAlignment p_horizontalAlignment, VerticalAlignment p_verticalAligment)
{
	_horizontalAlignment = p_horizontalAlignment;
	_verticalAlignment = p_verticalAligment;
	_alignmentChanged = true;
}

const glm::vec2& PanelTransformable::GetPosition() const
{
	return _position;
}

const glm::vec2& PanelTransformable::GetSize() const
{
	return _size;
}

HorizontalAlignment PanelTransformable::GetHorizontalAlignment() const
{
	return _horizontalAlignment;
}

VerticalAlignment PanelTransformable::GetVerticalAlignment() const
{
	return _verticalAlignment;
}

void PanelTransformable::Update()
{
	if (!_firstFrame)
	{
		if (!autoSize)
			UpdateSize();
		CopyImGuiSize();

		UpdatePosition();
		CopyImGuiPosition();
	}

	_firstFrame = false;
}

glm::vec2 PanelTransformable::CalculatePositionAlignmentOffset(bool p_default)
{
	glm::vec2 result(0.0f, 0.0f);

	switch (p_default ? _defaultHorizontalAlignment : _horizontalAlignment)
	{
	case HorizontalAlignment::CENTER:
		result.x -= _size.x / 2.0f;
		break;
	case HorizontalAlignment::RIGHT:
		result.x -= _size.x;
		break;
	}

	switch (p_default ? _defaultVerticalAlignment : _verticalAlignment)
	{
	case VerticalAlignment::MIDDLE:
		result.y -= _size.y / 2.0f;
		break;
	case VerticalAlignment::BOTTOM:
		result.y -= _size.y;
		break;
	}

	return result;
}

void PanelTransformable::UpdatePosition()
{
	if (_defaultPosition.x != -1.f && _defaultPosition.y != 1.f)
	{
		glm::vec2 offsettedDefaultPos = _defaultPosition + CalculatePositionAlignmentOffset(true);
		ImGui::SetWindowPos(ImVec2(offsettedDefaultPos.x, offsettedDefaultPos.y), _ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	}
	
	if (_positionChanged || _alignmentChanged)
	{
		glm::vec2 offset = CalculatePositionAlignmentOffset(false);
		glm::vec2 offsettedPos(_position.x + offset.x, _position.y + offset.y);
		ImGui::SetWindowPos(ImVec2(offsettedPos.x, offsettedPos.y), ImGuiCond_Always);
		_positionChanged = false;
		_alignmentChanged = false;
	}
}

void PanelTransformable::UpdateSize()
{
	if (_sizeChanged)
	{
		ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiCond_Always);
		_sizeChanged = false;
	}
}

void PanelTransformable::CopyImGuiPosition()
{
	ImVec2 go = ImGui::GetWindowPos();
	_position = glm::vec2(go.x, go.y);
}

void PanelTransformable::CopyImGuiSize()
{
	ImVec2 go = ImGui::GetWindowSize();
	_size = glm::vec2(go.x, go.y);
}

#pragma once

#include "Panel.h"
#include "UISettings.h"
#include "glm/glm.hpp"

namespace LindaEditor
{
	class PanelTransformable : public Panel
	{
	public:
		PanelTransformable
		(
			const glm::vec2& p_defaultPosition = glm::vec2(-1.f, -1.f),
			const glm::vec2& p_defaultSize = glm::vec2(-1.f, -1.f),
			HorizontalAlignment p_defaultHorizontalAlignment = HorizontalAlignment::LEFT,
			VerticalAlignment p_defaultVerticalAlignment = VerticalAlignment::TOP,
			bool p_ignoreConfigFile = false
		);

		void SetPosition(const glm::vec2& p_position);

		void SetSize(const glm::vec2& p_size);

		void SetAlignment(HorizontalAlignment p_horizontalAlignment, VerticalAlignment p_verticalAligment);

		const glm::vec2& GetPosition() const;

		const glm::vec2& GetSize() const;

		HorizontalAlignment GetHorizontalAlignment() const;

		VerticalAlignment GetVerticalAlignment() const;

	protected:
		void Update();
		virtual void _Draw_Impl() = 0;

	private:
		glm::vec2 CalculatePositionAlignmentOffset(bool p_default = false);

		void UpdatePosition();
		void UpdateSize();
		void CopyImGuiPosition();
		void CopyImGuiSize();

	public:
		bool autoSize = true;

	protected:
		glm::vec2 _defaultPosition;
		glm::vec2 _defaultSize;
		HorizontalAlignment _defaultHorizontalAlignment;
		VerticalAlignment _defaultVerticalAlignment;
		bool _ignoreConfigFile;

		glm::vec2 _position = glm::vec2(0.0f);
		glm::vec2 _size = glm::vec2(0.0f);

		bool _positionChanged = false;
		bool _sizeChanged = false;

		HorizontalAlignment _horizontalAlignment = HorizontalAlignment::LEFT;
		VerticalAlignment _verticalAlignment = VerticalAlignment::TOP;

		bool _alignmentChanged = false;
		bool _firstFrame = true;

	};
}

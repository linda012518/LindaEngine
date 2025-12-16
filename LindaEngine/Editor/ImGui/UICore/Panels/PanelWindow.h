#pragma once

#include "PanelTransformable.h"
#include "EventCallback.h"

namespace LindaEditor
{
	class PanelWindow : public PanelTransformable
	{
	public:
		PanelWindow(const std::string& p_name = "", bool p_opened = true, const PanelWindowSettings& p_panelSettings = PanelWindowSettings{});

		void Open();

		void Close();

		void Focus();

		void SetOpened(bool p_value);

		bool IsOpened() const;

		bool IsHovered() const;

		bool IsFocused() const;

		bool IsAppearing() const;

		bool IsVisible() const;

		void ScrollToBottom();

		void ScrollToTop();

		bool IsScrolledToBottom() const;

		bool IsScrolledToTop() const;

	protected:
		void _Draw_Impl() override;

	public:
		std::string name;

		glm::vec2 minSize = { 0.f, 0.f };
		glm::vec2 maxSize = { 0.f, 0.f };

		bool resizable = true;
		bool closable = false;
		bool movable = true;
		bool scrollable = true;
		bool dockable = false;
		bool hideBackground = false;
		bool forceHorizontalScrollbar = false;
		bool forceVerticalScrollbar = false;
		bool allowHorizontalScrollbar = false;
		bool bringToFrontOnFocus = true;
		bool collapsable = false;
		bool allowInputs = true;
		bool titleBar = true;

		LindaEngine::EventCallback<> OpenEvent;
		LindaEngine::EventCallback<> CloseEvent;

	private:
		bool _opened = false;
		bool _hovered = false;
		bool _focused = false;
		bool _mustScrollToBottom = false;
		bool _mustScrollToTop = false;
		bool _scrolledToBottom = false;
		bool _scrolledToTop = false;

	};
}

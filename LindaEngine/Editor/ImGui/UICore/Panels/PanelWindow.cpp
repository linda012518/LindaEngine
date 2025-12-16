#include "PanelWindow.h"
#include "IPlugin.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

using namespace LindaEditor;

PanelWindow::PanelWindow(const std::string& p_name, bool p_opened, const PanelWindowSettings& p_panelSettings)
	: name(p_name),
	resizable(p_panelSettings.resizable),
	closable(p_panelSettings.closable),
	movable(p_panelSettings.movable),
	scrollable(p_panelSettings.scrollable),
	dockable(p_panelSettings.dockable),
	hideBackground(p_panelSettings.hideBackground),
	forceHorizontalScrollbar(p_panelSettings.forceHorizontalScrollbar),
	forceVerticalScrollbar(p_panelSettings.forceVerticalScrollbar),
	allowHorizontalScrollbar(p_panelSettings.allowHorizontalScrollbar),
	bringToFrontOnFocus(p_panelSettings.bringToFrontOnFocus),
	collapsable(p_panelSettings.collapsable),
	allowInputs(p_panelSettings.allowInputs),
	_opened(p_opened)
{
	autoSize = p_panelSettings.autoSize;
}

void PanelWindow::Open()
{
	if (!_opened)
	{
		_opened = true;
		OpenEvent.Invoke();
	}
}

void PanelWindow::Close()
{
	if (_opened)
	{
		_opened = false;
		CloseEvent.Invoke();
	}
}

void PanelWindow::Focus()
{
	ImGui::SetWindowFocus((name + _panelID).c_str());
}

void PanelWindow::SetOpened(bool p_value)
{
	if (p_value != _opened)
	{
		_opened = p_value;

		if (_opened)
			OpenEvent.Invoke();
		else
			CloseEvent.Invoke();
	}
}

bool PanelWindow::IsOpened() const
{
	return _opened;
}

bool PanelWindow::IsHovered() const
{
	return _hovered;
}

bool PanelWindow::IsFocused() const
{
	return _focused;
}

bool PanelWindow::IsAppearing() const
{
	if (auto window = ImGui::FindWindowByName((name + GetPanelID()).c_str()); window)
		return window->Appearing;
	else
		return false;
}

bool PanelWindow::IsVisible() const
{
	if (auto window = ImGui::FindWindowByName((name + GetPanelID()).c_str()); window)
		return !window->Hidden;
	else
		return false;
}

void PanelWindow::ScrollToBottom()
{
	_mustScrollToBottom = true;
}

void PanelWindow::ScrollToTop()
{
	_mustScrollToTop = true;
}

bool PanelWindow::IsScrolledToBottom() const
{
	return _scrolledToBottom;
}

bool PanelWindow::IsScrolledToTop() const
{
	return _scrolledToTop;
}

void PanelWindow::_Draw_Impl()
{
	if (_opened)
	{
		int windowFlags = ImGuiWindowFlags_None;

		if (!resizable)					windowFlags |= ImGuiWindowFlags_NoResize;
		if (!movable)					windowFlags |= ImGuiWindowFlags_NoMove;
		if (!dockable)					windowFlags |= ImGuiWindowFlags_NoDocking;
		if (hideBackground)				windowFlags |= ImGuiWindowFlags_NoBackground;
		if (forceHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		if (forceVerticalScrollbar)		windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
		if (allowHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		if (!bringToFrontOnFocus)		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (!collapsable)				windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (!allowInputs)				windowFlags |= ImGuiWindowFlags_NoInputs;
		if (!scrollable)                windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		if (!titleBar)					windowFlags |= ImGuiWindowFlags_NoTitleBar;

		ImVec2 minSizeConstraint = ImVec2(minSize.x, minSize.y);
		ImVec2 maxSizeConstraint = ImVec2(maxSize.x, maxSize.y);

		/* Cancel constraint if x or y is <= 0.f */
		if (minSizeConstraint.x <= 0.f || minSizeConstraint.y <= 0.f)
			minSizeConstraint = { 0.0f, 0.0f };

		if (maxSizeConstraint.x <= 0.f || maxSizeConstraint.y <= 0.f)
			maxSizeConstraint = { 10000.f, 10000.f };

		ImGui::SetNextWindowSizeConstraints(minSizeConstraint, maxSizeConstraint);

		if (ImGui::Begin((name + _panelID).c_str(), closable ? &_opened : nullptr, windowFlags))
		{
			_hovered = ImGui::IsWindowHovered();
			_focused = ImGui::IsWindowFocused();

			auto scrollY = ImGui::GetScrollY();

			_scrolledToBottom = scrollY == ImGui::GetScrollMaxY();
			_scrolledToTop = scrollY == 0.0f;

			if (!_opened)
				CloseEvent.Invoke();

			Update();

			if (_mustScrollToBottom)
			{
				ImGui::SetScrollY(ImGui::GetScrollMaxY());
				_mustScrollToBottom = false;
			}

			if (_mustScrollToTop)
			{
				ImGui::SetScrollY(0.0f);
				_mustScrollToTop = false;
			}

			ExecutePlugins(PluginExecutionContext::PANEL);
			DrawWidgets();
		}

		ImGui::End();
	}

}

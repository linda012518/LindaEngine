#include "Widget.h"
#include "WidgetContainer.h"

#include "imgui/imgui.h"

using namespace LindaEditor;

uint64_t Widget::__WIDGET_ID_INCREMENT = 0;

Widget::Widget()
{
	_widgetID = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void Widget::Draw()
{
	if (enabled)
	{
		if (disabled)
		{
			ImGui::BeginDisabled();
		}

		_Draw_Impl();

		if (!tooltip.empty())
		{
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip(tooltip.c_str());
			}
		}

		if (disabled)
		{
			ImGui::EndDisabled();
		}

		if (_autoExecutePlugins)
			ExecutePlugins(PluginExecutionContext::WIDGET);

		if (!lineBreak)
		{
			ImGui::SameLine();
		}
	}

}

void Widget::LinkTo(const Widget& p_widget)
{
	_widgetID = p_widget._widgetID;
}

void Widget::Destroy()
{
	_destroyed = true;
}

bool Widget::IsDestroyed() const
{
	return _destroyed;
}

void Widget::SetParent(WidgetContainer* p_parent)
{
	_parent = p_parent;
}

bool Widget::HasParent() const
{
	return _parent;
}

WidgetContainer* Widget::GetParent()
{
	return _parent;
}

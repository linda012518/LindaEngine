#pragma once

#include "Widget.h"
#include "WidgetContainer.h"

#include "imgui/imgui.h"

#include <array>

namespace LindaEditor
{
	template <size_t _Size>
	class Columns : public Widget, public WidgetContainer
	{
	public:
		Columns()
		{
			widths.fill(-1.f);
		}

	protected:
		virtual void _Draw_Impl() override
		{
			ImGui::Columns(static_cast<int>(_Size), ("##" + _widgetID).c_str(), false);

			int counter = 0;

			CollectGarbages();

			for (auto it = _widgets.begin(); it != _widgets.end();)
			{
				it->first->Draw();

				++it;

				if (it != _widgets.end())
				{
					if (widths[counter] != -1.f)
						ImGui::SetColumnWidth(counter, widths[counter]);

					ImGui::NextColumn();
				}

				++counter;

				if (counter == _Size)
					counter = 0;
			}

			ImGui::Columns(1); // Necessary to not break the layout for following widget
		}

	public:
		std::array<float, _Size> widths;

	};
}

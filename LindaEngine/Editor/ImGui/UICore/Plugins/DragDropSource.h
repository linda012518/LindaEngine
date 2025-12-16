#pragma once

#include "IPlugin.h"
#include "EventCallback.h"
#include "imgui/imgui.h"

#include <string>

namespace LindaEditor
{
	template<typename T>
	class DragDropSource : public IPlugin
	{
	public:
		DragDropSource (const std::string& p_identifier, const std::string& p_tooltip, T p_data) 
			: identifier(p_identifier), tooltip(p_tooltip), data(p_data)
		{
		}

		virtual void Execute(EPluginExecutionContext p_context) override
		{
			ImGuiDragDropFlags src_flags = 0;
			src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
			src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging

			if (!hasTooltip)
				src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

			if (ImGui::BeginDragDropSource(src_flags))
			{
				if (!_isDragged)
					DragStartEvent.Invoke();

				_isDragged = true;

				if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
					ImGui::Text(tooltip.c_str());
				ImGui::SetDragDropPayload(identifier.c_str(), &data, sizeof(data));
				ImGui::EndDragDropSource();
			}
			else
			{
				if (_isDragged)
					DragStopEvent.Invoke();

				_isDragged = false;
			}
		}

		bool IsDragged() const
		{
			return _isDragged;
		}

	public:
		std::string identifier;
		std::string tooltip;
		T data;
		LindaEngine::EventCallback<> DragStartEvent;
		LindaEngine::EventCallback<> DragStopEvent;

		bool hasTooltip = true;

	private:
		bool _isDragged;
	};
}


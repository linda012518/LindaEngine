#pragma once

#include "IPlugin.h"
#include "EventCallback.h"

#include "imgui/imgui.h"

#include <string>

namespace LindaEditor
{
	template<typename T>
	class DragDropTarget : public IPlugin
	{
	public:
		DragDropTarget(const std::string& p_identifier) : identifier(p_identifier)
		{
		}

		virtual void Execute(EPluginExecutionContext p_context) override
		{
			const bool result = p_context == PluginExecutionContext::WIDGET ? ImGui::BeginDragDropTarget() :
				ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->WorkRect, ImGui::GetID(identifier.c_str()));

			if (result)
			{
				if (!_isHovered)
					HoverStartEvent.Invoke();

				_isHovered = true;

				ImGuiDragDropFlags target_flags = 0;
				// target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something

				if (!showYellowRect)
					target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(identifier.c_str(), target_flags))
				{
					T data = *(T*)payload->Data;
					DataReceivedEvent.Invoke(data);
				}
				ImGui::EndDragDropTarget();
			}
			else
			{
				if (_isHovered)
					HoverEndEvent.Invoke();

				_isHovered = false;
			}
		}

		bool IsHovered() const
		{
			return _isHovered;
		}

	public:
		std::string identifier;
		LindaEngine::EventCallback<T> DataReceivedEvent;
		LindaEngine::EventCallback<> HoverStartEvent;
		LindaEngine::EventCallback<> HoverEndEvent;

		bool showYellowRect = true;

	private:
		bool _isHovered;

	};
}

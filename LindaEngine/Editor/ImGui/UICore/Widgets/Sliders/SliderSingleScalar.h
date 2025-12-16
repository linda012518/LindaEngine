#pragma once

#include "DataWidget.h"
#include "SliderOrientation.h"
#include "EventCallback.h"
#include "ImguiConverter.h"

#include "glm/glm.hpp"
#include "imgui/imgui.h"

namespace LindaEditor
{
	template <typename T>
	class SliderSingleScalar : public DataWidget<T>
	{
		static_assert(std::is_scalar<T>::value, "Invalid SliderSingleScalar T (Scalar expected)");

	public:
		SliderSingleScalar
		(
			ImGuiDataType p_dataType,
			T p_min,
			T p_max,
			T p_value,
			SliderOrientation p_orientation,
			const std::string& p_label,
			const std::string& p_format
		) : DataWidget<T>(value), _dataType(p_dataType), min(p_min), max(p_max), value(p_value), orientation(p_orientation), label(p_label), format(p_format) {}

	protected:
		void _Draw_Impl() override
		{
			if (max < min)
				max = min;

			if (value < min)
				value = min;
			else if (value > max)
				value = max;

			bool valueChanged = false;

			switch (orientation)
			{
			case SliderOrientation::HORIZONTAL:
				valueChanged = ImGui::SliderScalar((label + this->_widgetID).c_str(), _dataType, &value, &min, &max, format.c_str());
				break;
			case SliderOrientation::VERTICAL:
				valueChanged = ImGui::VSliderScalar((label + this->_widgetID).c_str(), ImguiConverter::ToImVec2(verticalModeSize), _dataType, &value, &min, &max, format.c_str());
				break;
			}

			if (valueChanged)
			{
				ValueChangedEvent.Invoke(value);
				this->NotifyChange();
			}
		}

	public:
		T min;
		T max;
		T value;
		SliderOrientation orientation;
		glm::vec2 verticalModeSize; /* Only applied with ESliderOrientation::VERTICAL */
		std::string label;
		std::string format;
		LindaEngine::EventCallback<T> ValueChangedEvent;

	private:
		ImGuiDataType _dataType;

	};
}

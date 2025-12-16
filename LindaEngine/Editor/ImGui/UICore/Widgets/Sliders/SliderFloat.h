#pragma once

#include "SliderSingleScalar.h"

namespace LindaEditor
{
	class SliderFloat : public SliderSingleScalar<float>
	{
	public:
		SliderFloat
		(
			float p_min = 0.0f,
			float p_max = 1.0f,
			float p_value = 0.5f,
			SliderOrientation p_orientation = SliderOrientation::HORIZONTAL,
			const std::string& p_label = "",
			const std::string& p_format = "%.3f"
		);
	};
}

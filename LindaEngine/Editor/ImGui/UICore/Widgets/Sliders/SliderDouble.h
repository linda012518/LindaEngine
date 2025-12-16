#pragma once

#include "SliderSingleScalar.h"

namespace LindaEditor
{
	class SliderDouble : public SliderSingleScalar<double>
	{
	public:
		SliderDouble
		(
			double p_min = 0.0,
			double p_max = 1.0,
			double p_value = 0.5,
			SliderOrientation p_orientation = SliderOrientation::HORIZONTAL,
			const std::string& p_label = "",
			const std::string& p_format = "%.6f"
		);
	};
}

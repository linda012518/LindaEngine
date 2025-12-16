#pragma once

#include "SliderSingleScalar.h"

namespace LindaEditor
{
	class SliderInt : public SliderSingleScalar<int>
	{
	public:
		SliderInt
		(
			int p_min = 0,
			int p_max = 100,
			int p_value = 50,
			SliderOrientation p_orientation = SliderOrientation::HORIZONTAL,
			const std::string& p_label = "",
			const std::string& p_format = "%d"
		);
	};
}

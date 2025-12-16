#pragma once

#include "InputSingleScalar.h"

namespace LindaEditor
{
	class InputFloat : public InputSingleScalar<float>
	{
	public:
		InputFloat
		(
			float p_defaultValue = 0.0f,
			float p_step = 0.1f,
			float p_fastStep = 0.0f,
			const std::string& p_label = "",
			const std::string& p_format = "%.3f",
			bool p_selectAllOnClick = true
		);
	};
}

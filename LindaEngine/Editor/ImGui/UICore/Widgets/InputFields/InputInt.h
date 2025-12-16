#pragma once

#include "InputSingleScalar.h"

namespace LindaEditor
{
	class InputInt : public InputSingleScalar<int>
	{
	public:
		InputInt
		(
			int p_defaultValue = 0,
			int p_step = 1,
			int p_fastStep = 0,
			const std::string& p_label = "",
			const std::string& p_format = "%d",
			bool p_selectAllOnClick = true
		);
	};
}

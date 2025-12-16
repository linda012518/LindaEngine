#pragma once

#include <stdint.h>

namespace LindaEditor
{
	union TextureID
	{
		uint32_t id;
		void* raw;
	};
}
#pragma once

#include "Application.h"

namespace LindaEditor
{
	class ApplicationEditor : public LindaEngine::Application
	{
	public:
		int Initialize();
		void Finalize();
		void Tick();

	private:

	};
}


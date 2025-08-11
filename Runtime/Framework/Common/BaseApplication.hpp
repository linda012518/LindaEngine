#pragma once

#include "GfxConfiguration.h"
#include "../Interface/IApplication.hpp"

namespace LindaEngine
{
	class BaseApplication : implements IApplication
	{
	public:
		BaseApplication(GfxConfiguration& config);

		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		bool IsQuit() const;

	protected:
		static bool _quit;
		GfxConfiguration _config;

	private:
		BaseApplication() { }
	};
}



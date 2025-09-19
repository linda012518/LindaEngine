#include "BaseApplication.hpp"
#include "GraphicsContext.h"

using namespace LindaEngine;

bool BaseApplication::_quit = false;

BaseApplication::BaseApplication(GfxConfiguration& config)
	:_config(config)
{
}

int BaseApplication::Initialize()
{
	int ret = 0;

	if ((ret = g_GraphicsContext->Initialize()) != 0) {
		printf("GraphicsContext Initialize Failed...");
		return ret;
	}

	return ret;
}

void BaseApplication::Finalize()
{
	g_GraphicsContext->Finalize();
}

void BaseApplication::Tick()
{
	g_GraphicsContext->Tick();
}

bool BaseApplication::IsQuit() const
{
	return _quit;
}

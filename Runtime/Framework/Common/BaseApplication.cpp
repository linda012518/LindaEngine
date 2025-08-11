#include "BaseApplication.hpp"

using namespace LindaEngine;

bool BaseApplication::_quit = fasle;

BaseApplication::BaseApplication(GfxConfiguration& config)
	:_config(config)
{
}

int BaseApplication::Initialize()
{
	int ret = 0;

	return ret;
}

void BaseApplication::Finalize()
{
}

void BaseApplication::Tick()
{
}

bool BaseApplication::IsQuit() const
{
	return _quit;
}

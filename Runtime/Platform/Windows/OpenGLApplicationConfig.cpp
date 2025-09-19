#include "OpenGLApplication.hpp"
#include "Drivers/OpenGL/OpenglGraphicsContext.h"

namespace LindaEngine
{
	extern GfxConfiguration config;
	IApplication* g_pApp = static_cast<IApplication*>(new OpenGLApplication(config));
	GraphicsContext* g_GraphicsContext = static_cast<GraphicsContext*>(new OpenglGraphicsContext());
}
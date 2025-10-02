#include "OpenglAPIContext.h"
#include "glad/glad.h"

using namespace LindaEngine;

void OpenglAPIContext::SetViewport(int xStart, int yStart, int width, int height)
{
	glViewport(xStart, yStart, width, height);
}

void OpenglAPIContext::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void OpenglAPIContext::Clear(bool color, bool depth, bool stencil)
{
	int ret = 0;
	if (color) ret |= GL_COLOR_BUFFER_BIT;
	if (depth) ret |= GL_DEPTH_BUFFER_BIT;
	if (stencil) ret |= GL_STENCIL_BUFFER_BIT;

	glClear(ret);
}
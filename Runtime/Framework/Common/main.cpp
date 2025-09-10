#pragma comment(lib, "opengl32.lib")

#include <iostream>

#include "Platform/Windows/OpenGLApplication.hpp"

using namespace LindaEngine;



int main()
{
	GfxConfiguration config(8, 8, 8, 8, 24, 8, 0, 800, 600, "Test");

	IApplication* g_pApp = static_cast<IApplication*>(new OpenGLApplication(config));

	int ret;

	if ((ret = g_pApp->Initialize()) != 0) {
		printf("App Initialize failed, will exit now.");
		return ret;
	}

	while (!g_pApp->IsQuit()) {
		g_pApp->Tick();
	}

	g_pApp->Finalize();

	delete g_pApp;

	return 0;
}


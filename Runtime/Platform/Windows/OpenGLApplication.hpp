#pragma once

#include "WindowsApplication.hpp"

namespace LindaEngine
{
	class OpenGLApplication : public WindowsApplication
	{
    public:
        OpenGLApplication(GfxConfiguration& config);

        int Initialize() override;
        void Finalize() override;
        void Tick() override;

        void testInit();
        void testTick();

    private:
        HGLRC _renderContext;
	};
}



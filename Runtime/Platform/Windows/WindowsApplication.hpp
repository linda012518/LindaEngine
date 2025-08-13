#pragma once

#include <Windows.h>
#include "Framework/Common/BaseApplication.hpp"

namespace LindaEngine
{
	class WindowsApplication : public BaseApplication
	{
    public:
        WindowsApplication(GfxConfiguration& config);

        virtual int Initialize();
        virtual void Finalize();
        virtual void Tick();

        HWND GetMainWindow() { return _hWnd; };

    protected:
        void CreateMainWindow();

    private:
        static LRESULT CALLBACK WindowProc(HWND hWnd,
            UINT message,
            WPARAM wParam,
            LPARAM lParam);

    protected:
        HWND _hWnd;
        HDC  _hDc;
	};
}


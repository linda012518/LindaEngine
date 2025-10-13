#include "OpenglGraphicsContext.h"
#include "WinWindow.h"
#include "glad/glad_wgl.h"

#include <iostream>
#include <tchar.h>

using namespace LindaEngine;

static LRESULT CALLBACK TmpWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	return 0;
}

OpenglGraphicsContext::OpenglGraphicsContext(WinWindow* window)
{
	_window = window;
	_renderContext = NULL;
	_hDc = NULL;
}

int OpenglGraphicsContext::Initialize()
{
	int result;

	auto colorBits = graphicsConfig.redBits + graphicsConfig.greenBits + graphicsConfig.blueBits; // note on windows this does not include alpha bitplane

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = colorBits;
	pfd.cRedBits = graphicsConfig.redBits;
	pfd.cGreenBits = graphicsConfig.greenBits;
	pfd.cBlueBits = graphicsConfig.blueBits;
	pfd.cAlphaBits = graphicsConfig.alphaBits;
	pfd.cDepthBits = graphicsConfig.depthBits;
	pfd.cStencilBits = graphicsConfig.stencilBits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	//------------------------------------------------

	DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	WNDCLASSEX WndClassEx;
	memset(&WndClassEx, 0, sizeof(WNDCLASSEX));

	HINSTANCE hInstance = GetModuleHandle(NULL);

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc = TmpWndProc;
	WndClassEx.hInstance = hInstance;
	WndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClassEx.lpszClassName = _T("InitWindow");

	RegisterClassEx(&WndClassEx);
	HWND TemphWnd = CreateWindowEx(WS_EX_APPWINDOW, WndClassEx.lpszClassName, _T("InitWindow"), Style, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	HDC TemphDC = GetDC(TemphWnd);

	// Set a temporary default pixel format.
	int nPixelFormat = ChoosePixelFormat(TemphDC, &pfd);
	if (nPixelFormat == 0) return -1;

	result = SetPixelFormat(TemphDC, nPixelFormat, &pfd);
	if (result != 1)
	{
		return result;
	}

	// Create a temporary rendering context.
	_renderContext = wglCreateContext(TemphDC);
	if (!_renderContext)
	{
		printf("wglCreateContext failed!\n");
		return -1;
	}

	// Set the temporary rendering context as the current rendering context for this window.
	result = wglMakeCurrent(TemphDC, _renderContext);
	if (result != 1)
	{
		return result;
	}

	if (!gladLoadWGL(TemphDC)) {
		printf("WGL initialize failed!\n");
		result = -1;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(_renderContext);
	ReleaseDC(TemphWnd, TemphDC);
	DestroyWindow(TemphWnd);
	//---------------------------------------------------------


	_hDc = _window->GetWinDC();

	// now we try to init OpenGL Core profile context
	if (GLAD_WGL_ARB_pixel_format && GLAD_WGL_ARB_multisample && GLAD_WGL_ARB_create_context)
	{
		// enable MSAA
		const int attributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,     (int)colorBits,
			WGL_RED_BITS_ARB,		(int)graphicsConfig.redBits,
			WGL_GREEN_BITS_ARB,		(int)graphicsConfig.greenBits,
			WGL_BLUE_BITS_ARB,		(int)graphicsConfig.blueBits,
			WGL_ALPHA_BITS_ARB,		(int)graphicsConfig.alphaBits,
			WGL_DEPTH_BITS_ARB,     (int)graphicsConfig.depthBits,
			WGL_STENCIL_BITS_ARB,   (int)graphicsConfig.stencilBits,
			WGL_SAMPLE_BUFFERS_ARB, graphicsConfig.msaaSamples > 0 ? 1 : 0,  // 4x MSAA
			WGL_SAMPLES_ARB,        (int)graphicsConfig.msaaSamples,  // 4x MSAA
			0
		};

		UINT numFormats;
		int nPixelFormat;

		if (FAILED(wglChoosePixelFormatARB(_hDc, attributes, nullptr, 1, &nPixelFormat, &numFormats)) || numFormats == 0)
		{
			printf("wglChoosePixelFormatARB failed!\n");
			return -1;
		}

		result = SetPixelFormat(_hDc, nPixelFormat, &pfd);
		if (result != 1)
		{
			return result;
		}

		const int context_attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		_renderContext = wglCreateContextAttribsARB(_hDc, 0, context_attributes);
		if (!_renderContext)
		{
			printf("wglCreateContextAttributeARB failed!\n");
			return -1;
		}
	}
	else
	{
		// Set pixel format.
		int nPixelFormat = ChoosePixelFormat(_hDc, &pfd);
		if (nPixelFormat == 0) return -1;

		result = SetPixelFormat(_hDc, nPixelFormat, &pfd);
		if (result != 1)
		{
			return result;
		}

		// Create rendering context.
		_renderContext = wglCreateContext(_hDc);
		if (!_renderContext)
		{
			printf("wglCreateContext failed!\n");
			return -1;
		}
	}

	result = wglMakeCurrent(_hDc, _renderContext);
	if (result != 1)
	{
		return result;
	}

	//if (!gladLoadWGL(_hDc)) {
	//	printf("WGL initialize failed!\n");
	//	result = -1;
	//}
	//else {
	//	result = 0;
	//	printf("WGL initialize finished!\n");
	//}
	result = gladLoadGL();

	if (result != 1) {
		printf("Windows Application initialize failed!");
		return result;
	}

	return 0;

}

void OpenglGraphicsContext::Finalize()
{
	if (_renderContext) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_renderContext);
		_renderContext = 0;
	}
}

void OpenglGraphicsContext::Tick()
{
	SwapBuffers(_hDc);
}


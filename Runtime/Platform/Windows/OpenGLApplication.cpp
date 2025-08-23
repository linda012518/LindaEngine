#include "OpenGLApplication.hpp"

#include <tchar.h>

#include "glad/glad_wgl.h"

#include "Core/Entity.hpp"
#include "glm/glm.hpp"
#include "Component/Transform.h"

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

OpenGLApplication::OpenGLApplication(GfxConfiguration& config) 
	: WindowsApplication(config)
{
}

int OpenGLApplication::Initialize()
{
	int result;
	auto colorBits = _config.redBits + _config.greenBits + _config.blueBits; // note on windows this does not include alpha bitplane

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

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = colorBits;
	pfd.cRedBits = _config.redBits;
	pfd.cGreenBits = _config.greenBits;
	pfd.cBlueBits = _config.blueBits;
	pfd.cAlphaBits = _config.alphaBits;
	pfd.cDepthBits = _config.depthBits;
	pfd.cStencilBits = _config.stencilBits;
	pfd.iLayerType = PFD_MAIN_PLANE;

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
	else {
		result = 0;
		printf("WGL initialize finished!\n");
	}
	gladLoadGL();

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(_renderContext);
	ReleaseDC(TemphWnd, TemphDC);
	DestroyWindow(TemphWnd);

	// now initialize our application window
	WindowsApplication::CreateMainWindow();

	_hDc = GetDC(_hWnd);

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
			WGL_RED_BITS_ARB,		(int)_config.redBits,
			WGL_GREEN_BITS_ARB,		(int)_config.greenBits,
			WGL_BLUE_BITS_ARB,		(int)_config.blueBits,
			WGL_ALPHA_BITS_ARB,		(int)_config.alphaBits,
			WGL_DEPTH_BITS_ARB,     (int)_config.depthBits,
			WGL_STENCIL_BITS_ARB,   (int)_config.stencilBits,
			WGL_SAMPLE_BUFFERS_ARB, 1,  // 4x MSAA
			WGL_SAMPLES_ARB,        4,  // 4x MSAA
			0
		};

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = colorBits;
		pfd.cRedBits = _config.redBits;
		pfd.cGreenBits = _config.greenBits;
		pfd.cBlueBits = _config.blueBits;
		pfd.cAlphaBits = _config.alphaBits;
		pfd.cDepthBits = _config.depthBits;
		pfd.cStencilBits = _config.stencilBits;
		pfd.iLayerType = PFD_MAIN_PLANE;

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

		result = wglMakeCurrent(_hDc, _renderContext);
		if (result != 1)
		{
			return result;
		}

		result = 0; // we use 0 as success while OpenGL use 1, so convert it
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

		// Set the rendering context as the current rendering context for this window.
		result = wglMakeCurrent(_hDc, _renderContext);
		if (result != 1)
		{
			return result;
		}
	}

	result = BaseApplication::Initialize();

	if (result) {
		printf("Windows Application initialize failed!");
		return result;
	}

	return result;

}

void OpenGLApplication::Finalize()
{
	if (_renderContext) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_renderContext);
		_renderContext = 0;
	}

	WindowsApplication::Finalize();
}

void OpenGLApplication::Tick()
{
	WindowsApplication::Tick();

	glClearColor(0.0f, 0.3f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, _config.screenWidth, _config.screenHeight);

	Entity e("hello");

	Transform t;
	t.SetWorldPosition(glm::vec3(0.0, 1.0, 6.0));
	t.SetWorldRotation(glm::quat(0.1, 0.2, 0.3, 0.4));
	t.SetWorldScale(glm::vec3(1.0, 2.0, 1.0));

	std::cout << e << std::endl;

	SwapBuffers(_hDc);
}


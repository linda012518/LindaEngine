#include "OpenGLApplication.hpp"

#include <tchar.h>

#include "glad/glad_wgl.h"

#include "Core/Entity.hpp"
#include "glm/glm.hpp"
#include "Core/Transform.h"
#include "stb_image.h"
#include "Loader/TextLoader.h"
#include "Core/Shader.h"
#include "Component/PerspectiveCamera.h"

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

void OpenGLApplication::testInit()
{
	

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int texture, texture2;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Assets/Map/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		//当前绑定的纹理对象就会被附加上纹理图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//当前绑定的纹理自动生成所有需要的多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 设置包装uv两个方向的参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置滤波器
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小(Minify)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大(Magnify)

	stbi_set_flip_vertically_on_load(true);//翻转y轴
	data = stbi_load("Assets/Map/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//当前绑定的纹理对象就会被附加上纹理图像
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//当前绑定的纹理自动生成所有需要的多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);



	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);
}

void OpenGLApplication::testTick()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.3f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, _config.screenWidth, _config.screenHeight);

	std::string vs = TextLoader::Load("Assets/Shaders/texture.vs");
	std::string fs = TextLoader::Load("Assets/Shaders/texture.fs");
	Shader ourShader(vs.c_str(), fs.c_str());
	ourShader.Begin();

	ourShader.SetInt("texture1", 0);
	ourShader.SetInt("texture2", 1);
	ourShader.SetFloat("mixValue", 0.2f);

	Entity entityCamera("Camera");
	PerspectiveCamera* pc = entityCamera.AddComponent<PerspectiveCamera>(60.0f, (float)_config.screenWidth / (float)_config.screenHeight, 0.1f, 100.0f);
	entityCamera.GetTransform()->SetWorldPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	entityCamera.GetTransform()->Tick();

	glm::mat4 go1 = pc->GetProjectMatrix();
	glm::mat4 go2 = pc->GetViewMatrix();

	ourShader.SetMat4("projection", go1); // 
	ourShader.SetMat4("view", go2);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Entity entity;
	// 
	//glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		entity.GetTransform()->SetWorldPosition(cubePositions[i]);
		entity.GetTransform()->SetWorldEulerAngles(glm::vec3(-angle, 0, 0));
		entity.GetTransform()->Tick();
		glm::mat4 tempp = entity.GetTransform()->GetLocalToWorldMat();
		ourShader.SetMat4("model", tempp);
		//----------------------------------
		//glm::mat4 tempp1 = glm::mat4_cast(glm::quat(glm::vec3(angle, 0, 0)));
		//glm::mat4 model = glm::mat4(1.0f);
		//glm::mat4 tempp2 = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		//----------------------------------

		//glm::mat4 model = glm::mat4(1.0f);
		////if (i % 3 == 0 || i == 0)
		////	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 0, 1));
		//model = glm::translate(model, cubePositions[i]);
		////float angle = 20.0f * i;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		//ourShader.SetMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

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

	testInit();

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

	testTick();

	SwapBuffers(_hDc);
}


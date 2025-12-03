#include "ImGuiContextEditor.h"

#include "AutoPtr.h"
#include "Application.h"
#include "WinWindow.h"
#include "GraphicsContext.h"
#include "TextureManager.h"
#include "Graphic.h"

// 使用自定义的 glad loader，不使用 ImGui 自带的 loader
//#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
//
// 包含 glad 头文件（必须在 imgui_impl_opengl3.h 之前）
//#include "glad/glad.h"

#include "glad/glad_wgl.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace LindaEditor;
using namespace LindaEngine;

// Data stored per platform window for multi-viewport support
struct WGL_WindowData 
{ 
	HDC hDC; 
};

// Forward declarations - main OpenGL context shared by all viewports
static HGLRC g_MainRenderContext = nullptr;

// Support functions for multi-viewports
static void Hook_Renderer_CreateWindow(ImGuiViewport* viewport)
{
	if (viewport->RendererUserData != nullptr)
		return;

	WGL_WindowData* data = new WGL_WindowData();
	HWND hwnd = (HWND)viewport->PlatformHandle;
	
	// Get the device context for this window
	data->hDC = GetDC(hwnd);
	
	// Set pixel format to match the main window (required for OpenGL)
	// We need to get the pixel format from the main window's DC
	if (g_MainRenderContext != nullptr)
	{
		HDC mainDC = wglGetCurrentDC();
		if (mainDC != nullptr)
		{
			int pixelFormat = GetPixelFormat(mainDC);
			PIXELFORMATDESCRIPTOR pfd;
			DescribePixelFormat(mainDC, pixelFormat, sizeof(pfd), &pfd);
			SetPixelFormat(data->hDC, pixelFormat, &pfd);
		}
	}
	
	viewport->RendererUserData = data;
}

static void Hook_Renderer_DestroyWindow(ImGuiViewport* viewport)
{
	if (viewport->RendererUserData != nullptr)
	{
		WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData;
		if (data->hDC != nullptr)
		{
			ReleaseDC((HWND)viewport->PlatformHandle, data->hDC);
		}
		delete data;
		viewport->RendererUserData = nullptr;
	}
}

static void Hook_Platform_RenderWindow(ImGuiViewport* viewport, void*)
{
	// Activate the platform window DC in the main OpenGL rendering context
	// All viewports share the same OpenGL context
	if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
	{
		if (data->hDC != nullptr && g_MainRenderContext != nullptr)
		{
			wglMakeCurrent(data->hDC, g_MainRenderContext);
			
			// Set viewport for this window
			Graphic::SetViewport(0, 0, (int)viewport->Size.x, (int)viewport->Size.y);
		}
	}
}

static void Hook_Renderer_SwapBuffers(ImGuiViewport* viewport, void*)
{
	if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
	{
		if (data->hDC != nullptr)
		{
			::SwapBuffers(data->hDC);
		}
	}
}

int ImGuiContextEditor::Initialize()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	// Note: Docking and Viewports are only available in the 'docking' branch of ImGui
	// Current version (1.92.4 WIP) is the main branch which doesn't support these features

	io.Fonts->AddFontFromFileTTF("Assets/Fonts/Opensans/OpenSans-Bold.ttf", 18.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Opensans/OpenSans-Regular.ttf", 18.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Configure style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.WindowRounding = 0.0f;
	//style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	SetDarkThemeColors();

	WinWindow& window = dynamic_cast<WinWindow&>(Application::GetWindow());
	ImGui_ImplWin32_InitForOpenGL(window.GetHwnd());
	ImGui_ImplOpenGL3_Init();

	// Store main OpenGL context for multi-viewport support
	// All viewport windows will share this same OpenGL context
	g_MainRenderContext = wglGetCurrentContext();

	// Win32+GL needs specific hooks for viewport, as there are specific things needed to tie Win32 and GL api.
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
		platform_io.Renderer_CreateWindow = Hook_Renderer_CreateWindow;
		platform_io.Renderer_DestroyWindow = Hook_Renderer_DestroyWindow;
		platform_io.Renderer_SwapBuffers = Hook_Renderer_SwapBuffers;
		platform_io.Platform_RenderWindow = Hook_Platform_RenderWindow;
	}

	return 0;
}

void ImGuiContextEditor::Finalize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiContextEditor::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ImGuizmo::BeginFrame();
}

void ImGuiContextEditor::End()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)GraphicsContext::graphicsConfig.screenNewWidth, (float)GraphicsContext::graphicsConfig.screenNewHeight);

	RenderTextureManager::SetRenderTarget(nullptr);
	GraphicsConfig& config = GraphicsContext::graphicsConfig;
	Graphic::SetViewport(0, 0, config.screenNewWidth, config.screenNewHeight);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		// 保存当前OpenGL上下文
		HDC currentDC = wglGetCurrentDC();
		HGLRC currentContext = wglGetCurrentContext();
		
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		
		// 恢复OpenGL上下文
		wglMakeCurrent(currentDC, currentContext);
	}
}

void ImGuiContextEditor::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

void ImGuiContextEditor::OnImGuiRender()
{
	// 创建停靠空间 - 必须在任何窗口之前调用
	// 这会创建一个覆盖整个视口的停靠空间，窗口可以停靠到边缘
	// 使用ImGuiDockNodeFlags_PassthruCentralNode标志使中央节点透明，让3D渲染内容显示出来
	// 第一个参数是dockspace_id (ImGuiID类型，0表示使用默认ID)
	// 第二个参数是viewport (nullptr表示使用主视口)
	// 第三个参数是flags，使用PassthruCentralNode让中央区域透明
	ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	
	// 显示Demo窗口 - 使用&show_demo来允许关闭窗口
	static bool show_demo = true;
	if (show_demo)
	{
		// 设置窗口默认停靠到停靠空间（仅在首次使用时）
		ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		ImGui::ShowDemoWindow(&show_demo);
	}
}

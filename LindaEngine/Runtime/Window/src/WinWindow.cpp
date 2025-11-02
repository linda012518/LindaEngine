#include "WinWindow.h"
#include "Application.h"
#include "GraphicsContext.h"
#include "EventSystem.h"
#include "EventCode.h"
#include "Event.h"
#include <tchar.h>

using namespace LindaEngine;

WinWindow::WinWindow()
{ 
    _hWnd = NULL;
    _hDc = NULL;
}

int WinWindow::Initialize()
{
    // get the HINSTANCE of the Console Program
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("LindaEngine");
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    //wc.hIcon = (HICON)LoadImageW(GetModuleHandleW(NULL), L"glfw.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

    RegisterClassEx(&wc);

    GraphicsConfig& config = GraphicsContext::graphicsConfig;
    RECT window_rect = { 0, 0, (LONG)config.screenWidth,  (LONG)config.screenHeight };
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
    int NewWidth = window_rect.right - window_rect.left;
    int NewHeight = window_rect.bottom - window_rect.top;

    config.screenNewWidth = NewWidth;
    config.screenNewHeight = NewHeight;

    // create the window and use the result as the handle
    _hWnd = CreateWindowEx(0,
        _T("LindaEngine"),      // name of the window class
        config.appName,           // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        CW_USEDEFAULT,          // x-position of the window
        CW_USEDEFAULT,          // y-position of the window
        NewWidth,               // width of the window
        NewHeight,              // height of the window
        NULL,                   // we have no parent window, NULL
        NULL,                   // we aren't using menus, NULL
        hInstance,              // application handle
        this);                  // pass pointer to current object

    _hDc = GetDC(_hWnd);

    ShowWindow(_hWnd, SW_SHOW);
    UpdateWindow(_hWnd);

    return 0;
}

void WinWindow::Finalize()
{
    ReleaseDC(_hWnd, _hDc);
    DestroyWindow(_hWnd);
}

void WinWindow::Tick()
{
    // this struct holds Windows event messages
    MSG msg;

    // we use PeekMessage instead of GetMessage here
    // because we should not block the thread at anywhere
    // except the engine execution driver module 
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);

        // send the message to the WindowProc function
        DispatchMessage(&msg);
    }
}

HDC WinWindow::GetWinDC()
{
    return _hDc;
}

LRESULT WinWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WinWindow* pThis;
    if (message == WM_NCCREATE)
    {
        pThis = static_cast<WinWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        pThis = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return pThis->OnEvent(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);

}

LRESULT WinWindow::OnEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam)          (int)((short)HIWORD(wParam))
#endif

    switch (msg)
    {
    case WM_SIZE:
    {
        RECT    rt;
        GetClientRect(_hWnd, &rt);
        int width = rt.right - rt.left;
        int height = rt.bottom - rt.top;

        GraphicsContext::graphicsConfig.screenNewWidth = width;
        GraphicsContext::graphicsConfig.screenNewHeight = height;
        WindowResizeEvent event;
        event.width = width;
        event.height = height;
        EventSystem::Dispatch(nullptr, EventCode::WindowResize, event);
    }
    break;
    case WM_CHAR:
    {
        CharEvent event;
        event.key = (int)wParam;
        EventSystem::Dispatch(nullptr, EventCode::CharInput, event);
    }
    break;
    case WM_KEYUP:
    {
        KeyEvent event;
        event.key = (int)wParam;
        EventSystem::Dispatch(nullptr, EventCode::KeyUp, event);
    }
    break;
    case WM_KEYDOWN:
    {
        KeyEvent event;
        event.key = (int)wParam;
        EventSystem::Dispatch(nullptr, EventCode::KeyDown, event);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        MouseEvent event;
        event.x = GET_X_LPARAM(lParam);
        event.y = GET_Y_LPARAM(lParam);
        EventSystem::Dispatch(nullptr, EventCode::LeftMouseButtonDown, event);
    }
    break;
    case WM_LBUTTONUP:
    {
        MouseEvent event;
        event.x = GET_X_LPARAM(lParam);
        event.y = GET_Y_LPARAM(lParam);
        EventSystem::Dispatch(nullptr, EventCode::LeftMouseButtonUp, event);
    }
    break;
    case WM_RBUTTONDOWN:
    {
        MouseEvent event;
        event.x = GET_X_LPARAM(lParam);
        event.y = GET_Y_LPARAM(lParam);
        EventSystem::Dispatch(nullptr, EventCode::RightMouseButtonDown, event);
    }
    break;
    case WM_RBUTTONUP:
    {
        MouseEvent event;
        event.x = GET_X_LPARAM(lParam);
        event.y = GET_Y_LPARAM(lParam);
        EventSystem::Dispatch(nullptr, EventCode::RightMouseButtonUp, event);
    }
    break;
    case WM_MOUSEMOVE:
    {
        MouseEvent event;
        event.x = GET_X_LPARAM(lParam);
        event.y = GET_Y_LPARAM(lParam);
        EventSystem::Dispatch(nullptr, EventCode::MouseMove, event);
    }
    break;
    case WM_MOUSEWHEEL:
    {
        MouseEvent event;
        event.x = GET_X_LPARAM(lParam);
        event.y = GET_Y_LPARAM(lParam);
        event.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
        EventSystem::Dispatch(nullptr, EventCode::MouseWheel, event);
    }
    break;

    case WM_CLOSE:
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        Application::Quit();
    }
    break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return S_OK;
}


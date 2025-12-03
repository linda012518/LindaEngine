#include "WinWindow.h"
#include "Application.h"
#include "GraphicsContext.h"
#include "EventSystem.h"
#include "EventCode.h"
#include "Event.h"

#include "imgui/imgui_impl_win32.h"

#include <tchar.h>
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam)          (int)((short)HIWORD(wParam))
#endif

using namespace LindaEngine;

WinWindow::WinWindow()
{ 
    _hWnd = NULL;
    _hDc = NULL;
}

int WinWindow::Initialize()
{
    //感知dpi，缩放窗口大小
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    // get the HINSTANCE of the Console Program
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
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
    _leftButtonDown = false;
    _rightButtonDown = false;
    _leftButtonUp = false;
    _rightButtonUp = false;
    _keyDwon = KeyCode::None;
    _keyUp = KeyCode::None;

    // this struct holds Windows event messages
    MSG msg;

    // we use PeekMessage instead of GetMessage here
    // because we should not block the thread at anywhere
    // except the engine execution driver module 
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
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
        SetTimer(hWnd, 1, 100, NULL);
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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WinWindow::OnEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_ERASEBKGND:
        // 关键：阻止Windows擦除背景，保持OpenGL内容
        break;
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
        event.key = static_cast<KeyCode>((int)wParam);

        _keyHeld = false;
        auto clickDuration = std::chrono::steady_clock::now() - _keyStartTime;
        auto clickMillis = std::chrono::duration_cast<std::chrono::milliseconds>(clickDuration).count();

        if (clickMillis < CLICK_TIME_THRESHOLD) {
            EventSystem::Dispatch(nullptr, EventCode::KeyClick, event);
        }

        EventSystem::Dispatch(nullptr, EventCode::KeyUp, event);
        _keyUp = event.key;
        _key = KeyCode::None;
    }
    break;
    case WM_KEYDOWN:
    {
        KeyCode key = static_cast<KeyCode>((int)wParam);
        if (_key == key)
            break;

        _keyStartTime = std::chrono::steady_clock::now();
        _keyHeld = true;

        KeyEvent event;
        event.key = key;
        EventSystem::Dispatch(nullptr, EventCode::KeyDown, event);
        _keyDwon = key;
        _key = key;
    }
    break;
    case WM_LBUTTONDOWN: MouseButtonDown(lParam, true); break;
    case WM_LBUTTONUP: MouseButtonUp(lParam, true); break;
    case WM_RBUTTONDOWN: MouseButtonDown(lParam, false); break;
    case WM_RBUTTONUP: MouseButtonUp(lParam, false); break;
    case WM_MOUSEMOVE:
    {
        if (false == _mouseTracking)
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hWnd;
            tme.dwHoverTime = 0;

            if (TrackMouseEvent(&tme))
            {
                _mouseTracking = true;
            }
        }

        _mousePos.x = GET_X_LPARAM(lParam);
        _mousePos.y = GET_Y_LPARAM(lParam);
        MouseEvent event;
        event.x = _mousePos.x;
        event.y = _mousePos.y;
        EventSystem::Dispatch(nullptr, EventCode::MouseMove, event);
    }
    break;
    case WM_MOUSEWHEEL:
    {
        MouseEvent event;
        event.x = _mousePos.x;
        event.y = _mousePos.y;
        event.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
        EventSystem::Dispatch(nullptr, EventCode::MouseWheel, event);
    }
    break;
    case WM_MBUTTONDOWN:
    {
        _buttonStartTime = std::chrono::steady_clock::now();
        _wheelHeld = true;

        MouseEvent event;
        event.x = _mousePos.x;
        event.y = _mousePos.y;
        event.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
        EventSystem::Dispatch(nullptr, EventCode::MouseWheelDown, event);
    }
    break;
    case WM_MBUTTONUP:
    {
        _wheelHeld = false;

        MouseEvent event;
        event.x = _mousePos.x;
        event.y = _mousePos.y;
        event.wheel = GET_WHEEL_DELTA_WPARAM(wParam);

        auto clickDuration = std::chrono::steady_clock::now() - _buttonStartTime;
        auto clickMillis = std::chrono::duration_cast<std::chrono::milliseconds>(clickDuration).count();

        if (clickMillis < CLICK_TIME_THRESHOLD) {
            EventSystem::Dispatch(nullptr, EventCode::MouseWheelClick, event);
        }

        EventSystem::Dispatch(nullptr, EventCode::MouseWheelUp, event);
    }
    break;
    case WM_MOUSELEAVE:
    {
        _mouseTracking = false;
        MouseEvent event;
        EventSystem::Dispatch(nullptr, EventCode::MouseLeave, event);
    }
    break;
    case WM_TIMER:
        OnTimer(wParam, lParam);
    break;
    case WM_CLOSE:
    case WM_DESTROY:
    {
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        Application::Quit();
    }
    break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return S_OK;
}

void WinWindow::OnTimer(WPARAM wParam, LPARAM lParam)
{
    if (wParam != 1)
        return;

    CheckMouseButton(lParam);
    CheckMouseWheelPressed(lParam);

    auto holdDuration = std::chrono::steady_clock::now() - _keyStartTime;
    auto holdSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(holdDuration).count();
    if (holdSeconds < 300)
        return;
    // 300毫秒以上认为是按住

    KeyEvent event;
    event.key = _key;
    if (_keyHeld)
    {
        EventSystem::Dispatch(nullptr, EventCode::Key, event);
    }
}

void WinWindow::CheckMouseButton(LPARAM lParam)
{
    auto holdDuration = std::chrono::steady_clock::now() - _buttonStartTime;
    auto holdSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(holdDuration).count();
    if (holdSeconds < 300)
        return;
    // 300毫秒以上认为是按住

    MouseEvent event;
    event.x = GET_X_LPARAM(lParam);
    event.y = GET_Y_LPARAM(lParam);

    if (_wheelHeld)
    {
        EventSystem::Dispatch(nullptr, EventCode::MouseWheelPressed, event);
    }
}

void WinWindow::CheckMouseWheelPressed(LPARAM lParam)
{
    auto holdDuration = std::chrono::steady_clock::now() - _buttonStartTime;
    auto holdSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(holdDuration).count();
    if (holdSeconds < 300)
        return;
    // 300毫秒以上认为是按住

    MouseEvent event;
    event.x = GET_X_LPARAM(lParam);
    event.y = GET_Y_LPARAM(lParam);

    if (_leftButtonHeld)
    {
        _leftButton = true;
        EventSystem::Dispatch(nullptr, EventCode::LeftMouseButton, event);
    }
    else if (_rightButtonHeld)
    {
        _rightButton = true;
        EventSystem::Dispatch(nullptr, EventCode::RightMouseButton, event);
    }
}

void WinWindow::MouseButtonDown(LPARAM lParam, bool isLeft)
{
    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);

    if (isLeft)
    {
        _leftButtonDown = true;
        _leftButtonHeld = true;
    }
    else
    {
        _rightButtonDown = true;
        _rightButtonHeld = true;
    }

    _buttonStartTime = std::chrono::steady_clock::now();
    _downPoint.x = x;
    _downPoint.y = y;

    MouseEvent event;
    event.x = x;
    event.y = y;
    EventSystem::Dispatch(nullptr, isLeft ? EventCode::LeftMouseButtonDown : EventCode::RightMouseButtonDown, event);
}

void WinWindow::MouseButtonUp(LPARAM lParam, bool isLeft)
{
    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);

    if (isLeft)
    {
        _leftButtonUp = true;
        _leftButtonHeld = false;
        _leftButton = false;
    }
    else
    {
        _rightButtonUp = true;
        _rightButtonHeld = false;
        _rightButton = false;
    }

    MouseEvent event;
    event.x = x;
    event.y = y;

    auto clickDuration = std::chrono::steady_clock::now() - _buttonStartTime;
    auto clickMillis = std::chrono::duration_cast<std::chrono::milliseconds>(clickDuration).count();
    int moveDistance = abs(_downPoint.x - x) + abs(_downPoint.y - y);

    if (clickMillis < CLICK_TIME_THRESHOLD && moveDistance < CLICK_DISTANCE_THRESHOLD) {
        EventSystem::Dispatch(nullptr, isLeft ? EventCode::LeftMouseButtonClick : EventCode::RightMouseButtonClick, event);
    }

    EventSystem::Dispatch(nullptr, isLeft ? EventCode::LeftMouseButtonUp : EventCode::RightMouseButtonUp, event);
}


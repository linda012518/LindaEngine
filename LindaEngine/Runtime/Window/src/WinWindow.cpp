#include "WinWindow.h"
#include "Application.h"
#include <tchar.h>

using namespace LindaEngine;

WinWindow::WinWindow(GfxConfiguration& gfx)
{ 
    _gfx = gfx;
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

    RECT window_rect = { 0, 0, (LONG)_gfx.screenWidth,  (LONG)_gfx.screenHeight };
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
    int NewWidth = window_rect.right - window_rect.left;
    int NewHeight = window_rect.bottom - window_rect.top;

    _gfx.screenNewWidth = NewWidth;
    _gfx.screenNewHeight = NewHeight;

    // create the window and use the result as the handle
    _hWnd = CreateWindowEx(0,
        _T("LindaEngine"),      // name of the window class
        _gfx.appName,           // title of the window
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

GfxConfiguration& WinWindow::GetGfxConfig()
{
    return _gfx;
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
    }

    switch (message)
    {
    case WM_CHAR:
    {

    }
    break;
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_LEFT:

            break;
        case VK_RIGHT:

            break;
        case VK_UP:

            break;
        case VK_DOWN:

            break;

        default:
            break;
        }
    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:

            break;
        case VK_RIGHT:

            break;
        case VK_UP:

            break;
        case VK_DOWN:

            break;

        default:
            break;
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {

    }
    break;
    case WM_LBUTTONUP:
    {

    }
    break;
    case WM_MOUSEMOVE:

        break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        Application::Quit();
    }
    break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);

}


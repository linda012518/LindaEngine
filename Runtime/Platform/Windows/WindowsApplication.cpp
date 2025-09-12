#include "WindowsApplication.hpp"
#include <tchar.h>

using namespace LindaEngine;

WindowsApplication::WindowsApplication(GfxConfiguration& config) 
    : BaseApplication(config)
{

}

int WindowsApplication::Initialize()
{
    int result;

    CreateMainWindow();

    _hDc = GetDC(_hWnd);

    result = BaseApplication::Initialize();

    if (result != 0)
        exit(result);

    return result;
}

void WindowsApplication::Finalize()
{
    ReleaseDC(_hWnd, _hDc);

    BaseApplication::Finalize();
}

void WindowsApplication::Tick()
{
    BaseApplication::Tick();

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

void WindowsApplication::CreateMainWindow()
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
    //wc.hIcon = (HICON)LoadImageW(GetModuleHandleW(NULL), L"glfw.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

    // register the window class
    RegisterClassEx(&wc);

    RECT window_rect = { 0, 0, _config.screenWidth,  _config.screenHeight };
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
    int NewWidth = window_rect.right - window_rect.left;
    int NewHeight = window_rect.bottom - window_rect.top;

    // create the window and use the result as the handle
    _hWnd = CreateWindowEx(0,
        _T("LindaEngine"),      // name of the window class
        _config.appName,        // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        CW_USEDEFAULT,          // x-position of the window
        CW_USEDEFAULT,          // y-position of the window
        NewWidth,               // width of the window
        NewHeight,              // height of the window
        NULL,                   // we have no parent window, NULL
        NULL,                   // we aren't using menus, NULL
        hInstance,              // application handle
        this);                  // pass pointer to current object

    ShowWindow(_hWnd, SW_SHOW);

}

LRESULT WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WindowsApplication* pThis;
    if (message == WM_NCCREATE)
    {
        pThis = static_cast<WindowsApplication*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        pThis = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
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
                _quit = true;
            }
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

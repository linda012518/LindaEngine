#include "Application.h"

using namespace LindaEngine;

bool Application::_isQuit = false;
AppState Application::state = AppState::Loading;
Scope<Window> Application::_window;

void Application::SetFrameRate(int rate)
{
    _frameRate = rate;
    _frameInterval = 1.0f / _frameRate;
    _frameInterval *= 1000;
}

void Application::Quit()
{
    state = AppState::PrepareQuit;
    _isQuit = true;
}

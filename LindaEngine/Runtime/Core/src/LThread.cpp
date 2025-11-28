#include "LThread.h"

using namespace LindaEngine;

void LThread::Sleep(time_t dt)
{
	std::chrono::milliseconds t(dt);
	std::this_thread::sleep_for(t);
}

void LThread::Start(EventCall onCreate, EventCall onRun, EventCall onDestory)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!_isRun)
	{
		_isRun = true;

		if (onCreate)
			_onCreate = onCreate;
		if (onRun)
			_onRun = onRun;
		if (onDestory)
			_onDestory = onDestory;

		//线程
		std::thread t(std::mem_fn(&LThread::OnWork), this);
		t.detach();
	}
}

void LThread::Close()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_isRun)
	{
		_isRun = false;
		_sem.Wait();
	}
}

void LThread::Exit()
{
	if (_isRun)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_isRun = false;
	}
}

bool LThread::IsRun()
{
	return _isRun;
}

void LThread::OnWork()
{
	if (_onCreate)
		_onCreate(this);
	if (_onRun)
		_onRun(this);
	if (_onDestory)
		_onDestory(this);

	_sem.Wakeup();
	_isRun = false;
}

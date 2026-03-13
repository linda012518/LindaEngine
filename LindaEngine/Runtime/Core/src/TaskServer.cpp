#include "TaskServer.h"

using namespace LindaEngine;

void TaskServer::AddTask(Task task, std::function<void()> callback)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_tasksBuf.push_back({ std::move(task), std::move(callback) });
}

void TaskServer::Start()
{
	_thread.Start(nullptr, [this](LThread* pThread) {
		OnRun(pThread);
		}, nullptr);
}

void TaskServer::Close()
{
	_thread.Close();
}

void TaskServer::OnRun(LThread* pThread)
{
	while (pThread->IsRun())
	{
		//从缓冲区取出数据
		if (!_tasksBuf.empty())
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (auto item : _tasksBuf)
			{
				_tasks.push_back(std::move(item));
			}
			_tasksBuf.clear();
		}
		//如果没有任务
		if (_tasks.empty())
		{
			std::chrono::milliseconds t(1);
			std::this_thread::sleep_for(t);
			continue;
		}
		//处理任务
		for (auto item : _tasks)
		{
			if (item.task)
				item.task();
			if (item.callback)
				item.callback();
		}
		_tasks.clear();
	}
	//程序要退出了，看看缓冲队列里有没有，有的话再处理了
	for (auto item : _tasksBuf)
	{
		if (item.task)
			item.task();
		if (item.callback)
			item.callback();
	}
}



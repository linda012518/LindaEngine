#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <functional>

#include "LThread.h"

namespace LindaEngine
{
	class TaskServer
	{
		typedef std::function<void()> Task;

		struct TaskItem
		{
			Task task;
			std::function<void()> callback;
		};

	public:
		void AddTask(Task task, std::function<void()> callback = nullptr);
		void Start();
		void Close();

	protected:
		void OnRun(LThread* pThread);

	private:
		//任务数据
		std::list<TaskItem> _tasks;
		//任务数据缓冲区
		std::list<TaskItem> _tasksBuf;
		//改变数据缓冲区时需要加锁
		std::mutex _mutex;
		LThread _thread;

	};
}

#pragma once

#include "LSemaphore.h"

#include <functional>

namespace LindaEngine
{
	class LThread
	{
		typedef std::function<void(LThread*)> EventCall;

	public:
		static void Sleep(time_t dt);
		void Start(EventCall onCreate = nullptr, EventCall onRun = nullptr, EventCall onDestory = nullptr);
		void Close();

		//在工作函数中退出
		//不需要使用信号量来阻塞等待
		//如果使用会阻塞
		void Exit();

		//线程是否启动运行状态
		bool IsRun();

	protected:
		//线程的运行时的工作函数
		void OnWork();

	private:
		EventCall _onCreate;
		EventCall _onRun;
		EventCall _onDestory;
		//不同线程中改变数据时需要加锁
		std::mutex _mutex;
		//控制线程的终止、退出
		LSemaphore _sem;
		//线程是否启动运行中
		bool	_isRun = false;

	};
}

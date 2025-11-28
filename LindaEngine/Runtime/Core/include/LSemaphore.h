#pragma once

#include<chrono>
#include<thread>
#include<condition_variable>

namespace LindaEngine
{
	class LSemaphore
	{
	public:
		//阻塞当前线程
		void Wait();

		//唤醒当前线程
		void Wakeup();

	private:
		//改变数据缓冲区时需要加锁
		std::mutex _mutex;
		//阻塞等待-条件变量
		std::condition_variable _cv;
		//等待计数
		int _wait = 0;
		//唤醒计数
		int _wakeup = 0;

	};
}

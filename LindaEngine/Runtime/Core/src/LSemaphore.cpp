#include "LSemaphore.h"

using namespace LindaEngine;

void LSemaphore::Wait()
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (--_wait < 0)
	{
		//阻塞等待
		_cv.wait(lock, [this]()->bool {
			return _wakeup > 0;
			});
		--_wakeup;
	}
}

void LSemaphore::Wakeup()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (++_wait <= 0)
	{
		++_wakeup;
		_cv.notify_one();
	}
}

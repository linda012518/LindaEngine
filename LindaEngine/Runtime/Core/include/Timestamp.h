#pragma once

#include <chrono>

namespace LindaEngine
{
	class Timestamp
	{
	public:
		static void Initialize();
		static void Tick();

		static double GetElapsedSecond();//获取开始的总时长 - 秒
		static double GetElapsedMilliSecond();//获取开始的总时长 - 毫秒
		static long long GetElapsedMicroSecond();//获取开始的总时长 - 微秒

		static double GetDeltaSecond();//获取前一帧时间增量 - 秒
		static double GetDeltaMilliSecond();//获取前一帧时间增量 - 毫秒
		static long long GetDeltaMicroSecond();//获取前一帧时间增量 - 微秒

	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock> _beginTime;
		static std::chrono::time_point<std::chrono::high_resolution_clock> _lastTime;
	};
}




#include "Timestamp.h"

using namespace std::chrono;
using namespace LindaEngine;

std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp::_beginTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp::_lastTime;

void Timestamp::Initialize()
{
	_beginTime = high_resolution_clock::now();
	_lastTime = high_resolution_clock::now();
}

void Timestamp::Tick()
{
	_lastTime = high_resolution_clock::now();
}

double Timestamp::GetElapsedSecond()
{
	return GetElapsedMilliSecond() * 0.000001;
}

double Timestamp::GetElapsedMilliSecond()
{
	return GetElapsedMicroSecond() * 0.001;
}

long long Timestamp::GetElapsedMicroSecond()
{
	return duration_cast<microseconds>(high_resolution_clock::now() - _beginTime).count();
}

double Timestamp::GetDeltaSecond()
{
	return GetDeltaMilliSecond() * 0.000001;
}

double Timestamp::GetDeltaMilliSecond()
{
	return GetDeltaMicroSecond() * 0.001;
}

long long Timestamp::GetDeltaMicroSecond()
{
	return duration_cast<microseconds>(high_resolution_clock::now() - _lastTime).count();
}

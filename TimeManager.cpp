#include "pch.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
	prevFrameTime = time(nullptr);
}

TimeManager::~TimeManager()
{
}

void TimeManager::SetPrevFrameTime()
{
	prevFrameTime = time(nullptr);
}

time_t TimeManager::GetCurrentFrameTime()
{
	return time_t(nullptr);
}

time_t TimeManager::GetDeltaTime()
{
	return time(nullptr) - prevFrameTime;
}


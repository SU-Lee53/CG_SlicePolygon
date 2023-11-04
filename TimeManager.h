#pragma once
class TimeManager
{
	static TimeManager& GetInstance()
	{
		static TimeManager instance;
		return instance;
	}


	void SetPrevFrameTime();
	time_t GetCurrentFrameTime();
	time_t GetDeltaTime();


private:
	TimeManager();
	~TimeManager();

	time_t prevFrameTime = 0;
};


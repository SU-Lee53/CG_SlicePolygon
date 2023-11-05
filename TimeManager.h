#pragma once
class TimeManager
{
public:
	static TimeManager& GetInstance()
	{
		static TimeManager instance;
		return instance;
	}

	void Init();
	void Update();

	int GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	long long _frequency = 0;
	long long _prevCount = 0;
	float _deltaTime = 0.f;

private:
	int _frameCount = 0;
	float _frameTime = 0.f;
	int _fps = 0;

};


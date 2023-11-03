#pragma once
#include "SliceMain.h"

class Slicetool
{
public:
	static Slicetool& GetInstance()
	{
		static Slicetool instance;
		return instance;
	}

	void Slice(LINE_INFO sliceLine, Object* obj);
	// 도형의 각 변을 구해서 저장
	void GetObjectLineSegment(LINE_INFO* segmentBuf, Object* obj);

	// 교차점 함수
	float IsIntersect(LINE_INFO p1, LINE_INFO p2);
	glm::vec3 GetCrossingPoint(LINE_INFO p, float param);

private:
	Slicetool();
	~Slicetool();


};


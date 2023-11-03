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
	// ������ �� ���� ���ؼ� ����
	void GetObjectLineSegment(LINE_INFO* segmentBuf, Object* obj);

	// ������ �Լ�
	float IsIntersect(LINE_INFO p1, LINE_INFO p2);
	glm::vec3 GetCrossingPoint(LINE_INFO p, float param);

private:
	Slicetool();
	~Slicetool();


};


#pragma once
#include "SliceMain.h"

// 사실1 -> 나뉘는 도형: 변의 인덱스를 큰수에서 작은수를 뺀것이 더 작은쪽 도형이 가져갈 정점수
// 사실2 -> 가져갈 정점: 해당 인덱스에서 큰 수에서부터 갯수만큼 가져가면됨(빼면서)
//						나머지는 작은수부터 갯수만큼 가져가면됨 (빼면서)
// 사실3 -> 나눠지는 도형은 최대는 나눈 도형+1임. 오각형이면 최대 육각형으로도 나눠짐
// 사실 이게 다인듯...

class Slicetool
{
public:
	static Slicetool& GetInstance()
	{
		static Slicetool instance;
		return instance;
	}

	void Slice(LINE_INFO sliceLine, Object* obj, Object** Dest);
	

	// 교차점 함수
	bool IntersectCheck(LINE_INFO sliceLine, Object* obj);

private:
	// 도형의 각 변을 구해서 저장
	void SetObjectEdges(LINE_INFO* edgeBuf, Object* obj);

	// 교차점 함수
	float IsIntersect(LINE_INFO p1, LINE_INFO p2);
	glm::vec3 GetCrossingPoint(LINE_INFO p, float param);

private:
	Slicetool();
	~Slicetool();


};


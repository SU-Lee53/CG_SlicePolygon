#pragma once
#include "SliceMain.h"

// ���1 -> ������ ����: ���� �ε����� ū������ �������� ������ �� ������ ������ ������ ������
// ���2 -> ������ ����: �ش� �ε������� ū ���������� ������ŭ ���������(���鼭)
//						�������� ���������� ������ŭ ��������� (���鼭)
// ���3 -> �������� ������ �ִ�� ���� ����+1��. �������̸� �ִ� ���������ε� ������
// ��� �̰� ���ε�...

class Slicetool
{
public:
	static Slicetool& GetInstance()
	{
		static Slicetool instance;
		return instance;
	}

	void Slice(LINE_INFO sliceLine, Object* obj, Object** Dest);
	

	// ������ �Լ�
	bool IntersectCheck(LINE_INFO sliceLine, Object* obj);

private:
	// ������ �� ���� ���ؼ� ����
	void SetObjectEdges(LINE_INFO* edgeBuf, Object* obj);

	// ������ �Լ�
	float IsIntersect(LINE_INFO p1, LINE_INFO p2);
	glm::vec3 GetCrossingPoint(LINE_INFO p, float param);

private:
	Slicetool();
	~Slicetool();


};


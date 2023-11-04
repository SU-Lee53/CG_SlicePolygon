#include "pch.h"
#include "Slicetool.h"

Slicetool::Slicetool()
{
}

Slicetool::~Slicetool()
{
}

void Slicetool::Slice(LINE_INFO sliceLine, Object* obj)
{
	LINE_INFO lTemp[MAX_POINT];
	LINE_INFO* lines = lTemp;
	GetObjectLineSegment(lines, obj);

	for (int i = 0; i < obj->GetObjInfo().vCount; i++)
	{
		float param = IsIntersect(sliceLine, lines[i]);
		if (param == -1.0f)
			continue;
		else
		{
			cout << i << endl;
			glm::vec3 cp = GetCrossingPoint(sliceLine, param);
			cout << "Crossing Point: (" << cp[0] << ", " << cp[1] << ", " << cp[2] << ")" << endl;
			Coord pos = { cp[0], cp[1], cp[2] };
			
		}
	}
	cout << endl;
}

void Slicetool::GetObjectLineSegment(LINE_INFO* segmentBuf, Object* obj)
{
	LINE_INFO lineList[MAX_POINT];
	int vCount = obj->GetObjInfo().vCount;
	float* vBuf = obj->GetVAO().GetVertexBuffer();
	for (int i = 0; i < obj->GetObjInfo().vCount; i++)
	{
		int idx = i * 3;
		cout << vBuf[idx] << ' ' << vBuf[idx + 1] << ' ' << vBuf[idx + 2] << ' ' << endl;
	}
	for (int i = 0; i < vCount; i++)
	{
		int idx = i * 3;
		if (i == vCount - 1)
		{
			lineList[i].start = { vBuf[idx],  vBuf[idx+1], 0.0f };
			lineList[i].end = { vBuf[0],  vBuf[1], 0.0f };
		}
		else
		{
			lineList[i].start = { vBuf[idx],  vBuf[idx + 1], 0.0f };
			lineList[i].end = { vBuf[idx + 3],  vBuf[idx + 4], 0.0f };
		}
	}

	//	for (int i = 0; i < obj->GetObjInfo().vCount; i++)
	//	{
	//		cout << "start: " << lineList[i].start.x << ", " << lineList[i].start.y
	//			<< " end: " << lineList[i].end.x << ", " << lineList[i].end.y << endl;
	//	}
	//	cout << endl;

	copy(lineList, lineList + vCount, segmentBuf);

}

// p1 = p1.start
// p2 = p1.end
// p3 = p2.start
// p4 = p2.end
float Slicetool::IsIntersect(LINE_INFO p1, LINE_INFO p2)
{
	// 두 선분의 매개변수
	float t, s;
	float denominator = ((p1.end.x - p1.start.x) * (p2.end.y - p2.start.y) - (p1.end.y - p1.start.y) * (p2.end.x - p2.start.x));

	if (denominator == 0)
		return -1.0f;

	t = ((p2.start.x - p1.start.x) * (p2.end.y - p2.start.y) - (p2.start.y - p1.start.y) * (p2.end.x - p2.start.x)) / denominator;
	s = ((p1.start.y - p2.start.y) * (p1.end.x - p1.start.x) - (p1.start.x - p2.start.x) * (p1.end.y - p1.start.y)) / denominator;

	// 매개변수 t, s가 0 ~ 1 사이의 범위에 있을때 
	if (t >= 0 && t <= 1 && s >= 0 && s <= 1)
		return t;

	return -1.0f;
}

glm::vec3 Slicetool::GetCrossingPoint(LINE_INFO p, float param)
{
	glm::vec3 point;
	point[0] = p.start.x + param * (p.end.x - p.start.x);
	point[1] = p.start.y + param * (p.end.y - p.start.y);
	point[2] = 0.0f;

	return point;

}


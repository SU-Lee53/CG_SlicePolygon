#include "pch.h"
#include "Slicetool.h"

struct LINE_INFO;

Slicetool::Slicetool()
{
}

Slicetool::~Slicetool()
{
}

// ��ǻ� �� Ŭ������ ����
// ���ϰ����� ���� ���� 2���� �迭�� ���� ����
void Slicetool::Slice(LINE_INFO sliceLine, Object* obj, Object** Dest)
{
	LINE_INFO lTemp[MAX_POINT];
	LINE_INFO* lines = lTemp;
	SetObjectEdges(lines, obj);

	// cl: crossed line
	// cp: crossed point
	int cl[3];
	glm::vec3 cp[3];
	int idx = 0;

	for (int i = 0; i < obj->GetObjInfo().vCount; i++)
	{
		float param = IsIntersect(sliceLine, lines[i]);
		if (param == -1.0f)
			continue;
		else
		{
			cout << i << endl;
			cl[idx] = i;
			cp[idx] = GetCrossingPoint(sliceLine, param);
			cout << "Crossing Point: (" << cp[idx][0] << ", " << cp[idx][1] << ", " << cp[idx][2] << ")" << endl;
			if (idx++ == 2)
				break;
		}
	}

	// ���ݱ��� ������ �������� ���� ���ߴ�.
	// ���� �߶󺸵��� ����.

	// 1. �켱 ������ ���� �ε����� �������� �������� ���Ѵ�.
	// ���λ� n�� new
	int nIdx1[MAX_POINT + 1], nIdx2[MAX_POINT + 1];
	unsigned int nVcount1, nVcount2;
	nVcount1 = (cl[1] - cl[0]);
	nVcount2 = (obj->GetObjInfo().vCount - (cl[1] - cl[0]));

	// 1-1. ū ���� �ε������� ������ŭ �ٿ����� ��������
	for (int i = 0; i < nVcount1; i++)
	{
		nIdx1[i] = cl[1] - i;
		if (nIdx1[i] < 0)
		{
			nIdx1[i] += obj->GetObjInfo().vCount;
		}
	}

	// 1-2. ���� ���� �ε������� ������ŭ �÷����� ��������
	for (int i = 0; i < nVcount2; i++)
	{
		nIdx2[i] = cl[0] - i;
		if (nIdx2[i] < 0)
		{
			nIdx2[i] += obj->GetObjInfo().vCount;
		}
	}
	// �׽�Ʈ ���
	//	cout << "vCount1: " << nVcount1 << endl;
	//	cout << "index: ";
	//	for (int i = 0; i < nVcount1; i++)
	//	{
	//		cout << nIdx1[i] << ", ";
	//	}
	//	cout << endl;
	//	
	//	cout << "vCount2: " << nVcount2 << endl;
	//	cout << "index: ";
	//	for (int i = 0; i < nVcount2; i++)
	//	{
	//		cout << nIdx2[i] << ", ";
	//	}
	//	cout << endl;

	// 2. 1���� ���� ������ ���� �����. ������۴� �������� ���� �ɵ�
	// 2-1. �������� �����
	glm::vec3 temp1[(MAX_POINT + 1)];
	glm::vec3* vBuf1 = temp1;
	glm::vec3 temp2[(MAX_POINT + 1)];
	glm::vec3* vBuf2 = temp2;

	float* originVbuf = obj->GetObjInfo().vBuffer;

	int vIdx1 = 0;
	int vIdx2 = 0;

	for (vIdx1 = 0; vIdx1 < nVcount1; vIdx1++)
	{
		int oIdx = nIdx1[vIdx1] * 3;

		glm::vec3 vert = { originVbuf[oIdx], originVbuf[oIdx + 1], originVbuf[oIdx + 2] };
		vBuf1[vIdx1] = vert;
	}
	
	for (vIdx2 = 0; vIdx2 < nVcount2; vIdx2++)
	{
		int oIdx = nIdx2[vIdx2] * 3;

		glm::vec3 vert = { originVbuf[oIdx], originVbuf[oIdx + 1], originVbuf[oIdx + 2] };
		vBuf2[vIdx2] = vert;
	}


	for (int i = 0; i < 2; i++)
	{
		glm::vec3 cpvec = { cp[i][0], cp[i][1], cp[i][2] };
		vBuf1[vIdx1] = cpvec;
		vBuf2[vIdx2] = cpvec;
		vIdx1++;
		vIdx2++;
	}

	// 2-2. ���� ���� ��������
	RGB rgb = obj->GetObjInfo().objColor;

	// 3. ���� ����, �������� ������ ���ԵǾ� +2
	Object* newObj[2];
	Dest[0] = new Object(vBuf1, nVcount1 + 2, rgb, { 0.0f, 0.2f, 0.0f }, 0.8f);
	Dest[0]->SetObjectStatus(OS_FALLING);
	Dest[0]->SetFlyDist(obj->GetObjInfo().flyX, obj->GetObjInfo().flyY);
	//Dest[0]->SetFallDist(obj->GetObjInfo().fallDist);
	Dest[0]->SetFlyParam(obj->GetObjInfo().flyParam);

	Dest[1] = new Object(vBuf2, nVcount2 + 2, rgb, { 0.0f, 0.2f, 0.0f }, 0.8f);
	Dest[1]->SetObjectStatus(OS_FALLING);
	Dest[1]->SetFlyDist(obj->GetObjInfo().flyX, obj->GetObjInfo().flyY);
	//Dest[1]->SetFallDist(obj->GetObjInfo().fallDist);
	Dest[1]->SetFlyParam(obj->GetObjInfo().flyParam);

	// ��¦ ƨ���� ���ư� ������ ����
	int dir1 = 0, dir2= 0;
	for (int i = 0; i < nVcount1; i++)
	{
		if (nIdx1[i] == 0)
		{
			dir1 = 1;
			dir2 = -1;
			break;
		}
	}

	if (dir1 != 1)
	{
		dir1 = -1;
		dir2 = 1;
	}

	Dest[0]->SetMoveDirection(dir1);
	Dest[1]->SetMoveDirection(dir2);
}

bool Slicetool::IntersectCheck(LINE_INFO sliceLine, Object* obj)
{
	LINE_INFO lTemp[(MAX_POINT + 1)];
	LINE_INFO* lines = lTemp;
	SetObjectEdges(lines, obj);
	int count = 0;

	for (int i = 0; i < obj->GetObjInfo().vCount; i++)
	{
		if (IsIntersect(sliceLine, lines[i]) != -1.0f)
			count++;

		if (count >= 2)
			return true;
	}

	return false;
}

void Slicetool::SetObjectEdges(LINE_INFO* edgeBuf, Object* obj)
{
	LINE_INFO lineList[MAX_POINT];
	int vCount = obj->GetObjInfo().vCount;
	float* vBuf = obj->GetObjInfo().vBuffer;

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

	copy(lineList, lineList + vCount, edgeBuf);

}

float Slicetool::IsIntersect(LINE_INFO p1, LINE_INFO p2)
{
	// �� ������ �Ű�����
	float t, s;
	float denominator = ((p1.end.x - p1.start.x) * (p2.end.y - p2.start.y) - (p1.end.y - p1.start.y) * (p2.end.x - p2.start.x));

	if (denominator == 0)
		return -1.0f;

	t = ((p2.start.x - p1.start.x) * (p2.end.y - p2.start.y) - (p2.start.y - p1.start.y) * (p2.end.x - p2.start.x)) / denominator;
	s = ((p1.start.y - p2.start.y) * (p1.end.x - p1.start.x) - (p1.start.x - p2.start.x) * (p1.end.y - p1.start.y)) / denominator;

	// �Ű����� t, s�� 0 ~ 1 ������ ������ ������ 
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


#include "pch.h"
#include "Object.h"

Object::Object(glm::vec3* vertices, int vCount, RGB rgb)
{
	objInfo.objType = static_cast<OBJ_TYPE>(vCount);
	objInfo.objColor = rgb;
	objInfo.vCount = vCount;
	objInfo.pCount = vCount - 2;
	// 값 복사를 위한 임시 공간
	glm::vec3 temp[100];
	vBuf = temp;
	copy(vertices, vertices + vCount, vBuf);
	CreateFreeObject();
}

Object::~Object()
{
	_vao->~VAO();
}

void Object::CreateFreeObject()
{
	// 1. 좌표를 반시계방향 정렬
	SortVertexCCW();
	
	// vec3로 받은 정점을 vao에 넣기위해 float로 다시 버퍼 복사
	float temp[MAX_POINT * 3];		// 가능한 최대 정점수 미리 할당
	float* vBuff = temp;
	memcpy(vBuff, vBuf, sizeof(float) * objInfo.vCount * 3);

	// 2. 색상 버퍼 생성
	float cTemp[MAX_POINT * 3];		// 가능한 최대 정점수 미리 할당
	cBuf = cTemp;
	for (int i = 0; i < objInfo.vCount; i++)
	{
		int idx = i * 3;
		cBuf[idx] = objInfo.objColor.Red;
		cBuf[idx + 1] = objInfo.objColor.Green;
		cBuf[idx + 2] = objInfo.objColor.Blue;
	}

	// 3. 인덱스 버퍼 생성
	// 3-1. 삼각형의 경우 그냥 {0, 1, 2}로 초기화가 가장 best
	// 3-2. 그 이상의 경우 정해진 순서대로 버퍼를 채움
	unsigned int iTemp[(MAX_POINT - 2) * 3];		// 가능한 최대 정점수 미리 할당
	idxBuf = iTemp;
	if (objInfo.vCount <= 3)
	{
		idxBuf[0] = 0;
		idxBuf[1] = 1;
		idxBuf[2] = 2;
	}
	else
	{
		for (int i = 0; i < objInfo.pCount; i++)
		{
			int idx = i * 3;
			idxBuf[idx] = 0;
			idxBuf[idx + 1] = i + 1;
			idxBuf[idx + 2] = i + 2;
		}
	}

	// 4. vao 바인드
	_vao = new VAO(vBuff, cBuf, idxBuf, objInfo.vCount * 3, objInfo.pCount * 3);

}

void Object::SortVertexCCW()
{
	// 올바르게 정렬될수 있는 조건: 도형이 반드시 볼록해야함, 오목한 부분이 있다면 정렬 불가
	
	// 정렬 방법
	// 1. 모든 정점의 중심점(평균)을 구한다.
	// 2. 중심점보다 y축으로 윗부분과 아랫부분을 나눈다.
	// 3. 윗부분은 x축 내림차순, 아랫부분은 오름차순으로 연결한다.
	// 4. 두 배열을 합친다.

	// 1. 중심점을 구한다. y값의 중심만 구하면 될듯함
	float middleY = 0.0;
	for (int i = 0; i < objInfo.vCount; i++)
	{
		middleY += vBuf[i][1];
	}
	middleY = middleY / objInfo.vCount;

	// 2. 중심점보다 윗부분과 아랫부분을 나눈다
	vector<glm::vec3> upper;
	vector<glm::vec3> lower;
	
	for (int i = 0; i < objInfo.vCount; i++)
	{
		if (vBuf[i][1] >= middleY)
			upper.push_back(vBuf[i]);
		else
			lower.push_back(vBuf[i]);
	}

	// 3. 윗부분은 x축 내림차순, 아랫부분은 오름차순으로 연결한다.
	sort(upper.begin(), upper.end(), Object::CompareDescendingX);
	sort(lower.begin(), lower.end(), Object::CompareAscendingX);

	// 4. 원래 배열에 합친다. copy로 복사
	upper.insert(upper.end(), lower.begin(), lower.end());
	copy(upper.begin(), upper.end(), vBuf);

	// 벡터 메모리 헤제
	vector<glm::vec3>().swap(upper);
	vector<glm::vec3>().swap(lower);
}

void Object::Render()
{
	glBindVertexArray(_vao->GetVAOHandle());
	glDrawElements(GL_TRIANGLES, _vao->GetElementCount(), GL_UNSIGNED_INT, 0);
}

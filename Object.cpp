#include "pch.h"
#include "Object.h"

Object::Object(glm::vec3* vertices, int vCount, RGB rgb, glm::vec3 startP, float endY)
{
	objInfo.objType = static_cast<OBJ_TYPE>(vCount);
	objInfo.objColor = rgb;
	objInfo.vCount = vCount;
	objInfo.pCount = vCount - 2;
	objInfo.startP = glm::vec3(1.5f, startP[1], 0.0f);
	objInfo.endP = glm::vec3(-1.5f, endY, 0.0f);
	// 값 복사를 위한 임시 공간
	glm::vec3 temp[100];
	vBuf = temp;
	memcpy(objInfo.vBuffer, vertices, sizeof(float) * objInfo.vCount * 3);
	GetCenter();
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
	float temp[(MAX_POINT + 1) * 3];		// 가능한 최대 정점수 미리 할당
	float* vBuff = temp;
	memcpy(vBuff, vBuf, sizeof(float) * objInfo.vCount * 3);

	// 2. 색상 버퍼 생성
	float cTemp[(MAX_POINT + 1) * 3];		// 가능한 최대 정점수 미리 할당
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
	unsigned int iTemp[(MAX_POINT + 1 - 2) * 3];		// 가능한 최대 정점수 미리 할당
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

void Object::GetCenter()
{
	float centerX = 0.0f;
	float centerY = 0.0f;
	for (int i = 0; i < objInfo.vCount; i++)
	{
		int idx = i * 3;
		centerX += objInfo.vBuffer[idx];
		centerY += objInfo.vBuffer[idx + 1];
	}
	centerX = centerX / objInfo.vCount;
	centerY = centerY / objInfo.vCount;

	objInfo.centerPos = { centerX, centerY, 0.0f };

}

void Object::Render()
{
	glBindVertexArray(_vao->GetVAOHandle());
	glDrawElements(GL_TRIANGLES, _vao->GetElementCount(), GL_UNSIGNED_INT, 0);
}


// 떨어질때 사용할 함수

void Object::FlyingUpdate()
{
	// parametric equation으로 파라미터가 1.0f가 되면 끝임

	// objInfo.state가 떨어지는 상황이라면 날아가는 속도를 줄여서 0으로 만듬
	if (objInfo.state == OS_FALLING)
	{
		objInfo.flySpeed -= 0.000001f;
	}
	objInfo.flyparam += objInfo.flySpeed;
	glm::vec3 dist = GetFlyingDistance(objInfo.flyparam);
	objInfo.flyMat = GET_SINGLE(TransformManager).GetTranslateMatrix(dist);

	FinalMatUpdate();
}

glm::vec3 Object::GetFlyingDistance(float flyparam)
{
	float distX = ((1.0f - flyparam) * objInfo.startP[0]) + (flyparam * (objInfo.endP[0]));
	float distY = ((1.0f - flyparam) * objInfo.startP[1]) + (flyparam * (objInfo.endP[1]));

	return glm::vec3(distX, distY, 0.0f);
}

void Object::GravityUpdate()
{
	objInfo.fallDist -= objInfo.fallSpeed;
	objInfo.fallMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(0.0f, objInfo.fallDist, 0.0f));
	objInfo.fallSpeed += 0.0000005f;

	FinalMatUpdate();
}

void Object::FinalMatUpdate()
{
	objInfo.finalMat = objInfo.flyMat * objInfo.fallMat;
	vBufferUpdate();
}

void Object::vBufferUpdate()
{
	for (int i = 0; i < objInfo.vCount; i++)
	{
		int vidx = i * 3;
		float* vBuff = _vao->GetVertexBuffer();
		glm::vec4 vTemp = glm::vec4(vBuff[vidx], vBuff[vidx + 1], vBuff[vidx + 2], 1.0f);
		vTemp = objInfo.finalMat * vTemp;
		objInfo.vBuffer[vidx] = vTemp[0];
		objInfo.vBuffer[vidx + 1] = vTemp[1];
		objInfo.vBuffer[vidx + 2] = vTemp[2];
	}

	// 하는김에 중심점도 업데이트
	GetCenter();
}

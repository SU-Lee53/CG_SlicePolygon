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
	// **수정: 아래 정점을 나누고 정렬하는 부분을 위아래로만 하지 않고 사분면으로 나눠야 정상적으로 작동할것으로 보임
	
	// 1. 중심점을 구한다.
	float middleX = 0.0f;
	float middleY = 0.0f;
	for (int i = 0; i < objInfo.vCount; i++)
	{
		middleX += vBuf[i][0];
		middleY += vBuf[i][1];
	}
	middleX = middleX / objInfo.vCount;
	middleY = middleY / objInfo.vCount;
	GetCenter();

	// 2. 중심점 기준 사분면으로 나눈다

	vector<glm::vec3> quad1;
	vector<glm::vec3> quad2;
	vector<glm::vec3> quad3;
	vector<glm::vec3> quad4;

	for (int i = 0; i < objInfo.vCount; i++)
	{
		if (vBuf[i][0] >= middleX && vBuf[i][1] >= middleY)
			quad1.push_back(vBuf[i]);
		else if (vBuf[i][0] < middleX && vBuf[i][1] >= middleY)
			quad2.push_back(vBuf[i]);
		else if (vBuf[i][0] <= middleX && vBuf[i][1] < middleY)
			quad3.push_back(vBuf[i]);
		else if (vBuf[i][0] > middleX && vBuf[i][1] < middleY)
			quad4.push_back(vBuf[i]);
	}

	// 3. 사분면별 정렬을 수행
	// 3-1. 1사분면: x축 내림차순, 값이 같으면 y축 오름차순
	// 3-2. 2사분면: x축 내림차순, 값이 같으면 y축 내림차순
	// 3-3. 3사분면: x축 오름차순, 값이 같으면 y축 내림차순
	// 3-4. 4사분면: x축 오름차순, 값이 같으면 y축 오름차순
	
	sort(quad1.begin(), quad1.end(), Object::CompareQuad1);
	sort(quad2.begin(), quad2.end(), Object::CompareQuad2);
	sort(quad3.begin(), quad3.end(), Object::CompareQuad3);
	sort(quad4.begin(), quad4.end(), Object::CompareQuad4);

	// 4. 원래 배열에 합친다. copy로 복사
	quad1.insert(quad1.end(), quad2.begin(), quad2.end());
	quad1.insert(quad1.end(), quad3.begin(), quad3.end());
	quad1.insert(quad1.end(), quad4.begin(), quad4.end());
	copy(quad1.begin(), quad1.end(), vBuf);
	memcpy(objInfo.vBuffer, vBuf, sizeof(float) * objInfo.vCount * 3);


	// 벡터 메모리 헤제
	vector<glm::vec3>().swap(quad1);
	vector<glm::vec3>().swap(quad2);
	vector<glm::vec3>().swap(quad3);
	vector<glm::vec3>().swap(quad4);
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

// 날아갈때 사용할 함수

void Object::FlyingUpdate()
{
	deltaT = GET_SINGLE(TimeManager).GetDeltaTime();
	// 날아가기
	// parametric equation으로 파라미터가 1.0f가 되면 끝임
	objInfo.flyParam += objInfo.flySpeed * deltaT;
	glm::vec3 dist = GetFlyingDistance(objInfo.flyParam);
	objInfo.flyMat = GET_SINGLE(TransformManager).GetTranslateMatrix(dist);

	// 회전하기
	objInfo.rotDeg += objInfo.rotSpeed * deltaT;
	objInfo.rotMat = GET_SINGLE(TransformManager).GetRotateMatrix(objInfo.rotDeg, Z_AXIS);

	FinalMatUpdate();
}

glm::vec3 Object::GetFlyingDistance(float flyParam)
{
	objInfo.flyDist[0] = ((1.0f - flyParam) * objInfo.startP[0]) + (flyParam * (objInfo.endP[0]));
	objInfo.flyDist[1] = ((1.0f - flyParam) * objInfo.startP[1]) + (flyParam * (objInfo.endP[1]));
	
	return glm::vec3(objInfo.flyDist[0], objInfo.flyDist[1], 0.0f);
}

// 떨어질때 사용할 함수

void Object::GravityUpdate()
{
	deltaT = GET_SINGLE(TimeManager).GetDeltaTime();

	objInfo.fallDist -= objInfo.fallSpeed * deltaT;
	objInfo.fallMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(0.0f, objInfo.fallDist, 0.0f));
	objInfo.fallSpeed += 0.001f;
	
	FinalMatUpdate();
}

void Object::InBasketUpdate(Basket* basket)
{
	deltaT = GET_SINGLE(TimeManager).GetDeltaTime();
	objInfo.ibMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(basket->GetBasketMoveDist(), objInfo.flyDist[1] - (-objInfo.fallDist), 0.0f));

	FinalMatUpdate();
}

void Object::FinalMatUpdate()
{
	objInfo.finalMat = objInfo.ibMat * objInfo.flyMat * objInfo.fallMat * objInfo.rotMat;
	vBufferUpdate(objInfo.finalMat);
}

void Object::vBufferUpdate(glm::mat4 matrix)
{
	for (int i = 0; i < objInfo.vCount; i++)
	{
		int vidx = i * 3;
		float* vBuff = _vao->GetVertexBuffer();
		glm::vec4 vTemp = glm::vec4(vBuff[vidx], vBuff[vidx + 1], vBuff[vidx + 2], 1.0f);
		vTemp = matrix * vTemp;
		objInfo.vBuffer[vidx] = vTemp[0];
		objInfo.vBuffer[vidx + 1] = vTemp[1];
		objInfo.vBuffer[vidx + 2] = vTemp[2];
	}

	// 하는김에 중심점도 업데이트
	GetCenter();
}

#include "pch.h"
#include "Object.h"

glm::vec3 Object::standardP = glm::vec3(0.0f, 0.0f, 0.0f);

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
	Object::standardP = vBuf[0];
	sort(vBuf + 1, vBuf + objInfo.vCount, Object::Compare);
}

bool Object::Compare(const glm::vec3 p1, const glm::vec3 p2)
{
	int direction = IsCCW(Object::standardP, p1, p2);

	if (direction == 0)
	{
		return (GetDist(Object::standardP, p1) < GetDist(Object::standardP, p2));
	}
	else if (direction == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Object::IsCCW(const glm::vec3 stdP, const glm::vec3 p1, const glm::vec3 p2)
{
	float crossValue = ((p1.x - stdP.x) * (p2.y - stdP.y)) - ((p2.x - stdP.x) * (p1.y - stdP.y));

	if (crossValue > 0)
		return 1;
	else if (crossValue < 0)
		return -1;
	else
		return 0;
}

float Object::GetDist(const glm::vec3 p1, const glm::vec3 p2)
{
	float dist = glm::pow((p1.x - p2.x), 2) + glm::pow((p1.y - p2.y), 2);
	return dist;
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

///////////////////////
// 날아갈때 사용할 함수
///////////////////////
void Object::FlyingUpdate()
{
	deltaT = GET_SINGLE(TimeManager).GetDeltaTime();
	// 날아가기
	// parametric equation으로 파라미터가 1.0f가 되면 끝임
	objInfo.flyParam += objInfo.flySpeed * deltaT;
	glm::vec3 pos = GetFlyingDistance(objInfo.flyParam);
	objInfo.flyMat = GET_SINGLE(TransformManager).GetTranslateMatrix(pos);

	// 회전하기
	objInfo.rotDeg += objInfo.rotSpeed * deltaT;
	objInfo.rotMat = GET_SINGLE(TransformManager).GetRotateMatrix(objInfo.rotDeg, Z_AXIS);

	FinalMatUpdate();
}

glm::vec3 Object::GetFlyingDistance(float flyParam)
{
	objInfo.flyX = ((1.0f - flyParam) * objInfo.startP[0]) + (flyParam * (objInfo.endP[0]));
	objInfo.flyY = ((1.0f - flyParam) * objInfo.startP[1]) + (flyParam * (objInfo.endP[1]));
	
	return glm::vec3(objInfo.flyX, objInfo.flyY, 0.0f);
}

///////////////////////
// 떨어질때 사용할 함수
///////////////////////
void Object::GravityUpdate()
{
	deltaT = GET_SINGLE(TimeManager).GetDeltaTime();

	objInfo.fallDist -= objInfo.fallSpeed * deltaT;
	objInfo.fallMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(0.0f, objInfo.fallDist, 0.0f));
	objInfo.fallSpeed += 0.01f;
	
	FinalMatUpdate();
}


////////////////////////////////
// 바구니에 담겼을때 사용할 함수
////////////////////////////////
void Object::InBasketUpdate(Basket* basket)
{
	deltaT = GET_SINGLE(TimeManager).GetDeltaTime();
	if (basket->GetBasketMoveDirection())
	{
		objInfo.moveDist += basket->GetBasketMoveSpeed() * deltaT;
		objInfo.ibMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(objInfo.moveDist, 0.0f, 0.0f));
	}
	else
	{
		objInfo.moveDist -= basket->GetBasketMoveSpeed() * deltaT;
		objInfo.ibMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(objInfo.moveDist, 0.0f, 0.0f));
	}

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

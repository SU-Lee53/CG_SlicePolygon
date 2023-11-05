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
	// �� ���縦 ���� �ӽ� ����
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
	// 1. ��ǥ�� �ݽð���� ����
	SortVertexCCW();
	
	// vec3�� ���� ������ vao�� �ֱ����� float�� �ٽ� ���� ����
	float temp[(MAX_POINT + 1) * 3];		// ������ �ִ� ������ �̸� �Ҵ�
	float* vBuff = temp;
	memcpy(vBuff, vBuf, sizeof(float) * objInfo.vCount * 3);

	// 2. ���� ���� ����
	float cTemp[(MAX_POINT + 1) * 3];		// ������ �ִ� ������ �̸� �Ҵ�
	cBuf = cTemp;
	for (int i = 0; i < objInfo.vCount; i++)
	{
		int idx = i * 3;
		cBuf[idx] = objInfo.objColor.Red;
		cBuf[idx + 1] = objInfo.objColor.Green;
		cBuf[idx + 2] = objInfo.objColor.Blue;
	}

	// 3. �ε��� ���� ����
	// 3-1. �ﰢ���� ��� �׳� {0, 1, 2}�� �ʱ�ȭ�� ���� best
	// 3-2. �� �̻��� ��� ������ ������� ���۸� ä��
	unsigned int iTemp[(MAX_POINT + 1 - 2) * 3];		// ������ �ִ� ������ �̸� �Ҵ�
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

	// 4. vao ���ε�
	_vao = new VAO(vBuff, cBuf, idxBuf, objInfo.vCount * 3, objInfo.pCount * 3);

}

void Object::SortVertexCCW()
{
	// �ùٸ��� ���ĵɼ� �ִ� ����: ������ �ݵ�� �����ؾ���, ������ �κ��� �ִٸ� ���� �Ұ�
	
	// ���� ���
	// 1. ��� ������ �߽���(���)�� ���Ѵ�.
	// 2. �߽������� y������ ���κа� �Ʒ��κ��� ������.
	// 3. ���κ��� x�� ��������, �Ʒ��κ��� ������������ �����Ѵ�.
	// 4. �� �迭�� ��ģ��.
	// **����: �Ʒ� ������ ������ �����ϴ� �κ��� ���Ʒ��θ� ���� �ʰ� ��и����� ������ ���������� �۵��Ұ����� ����
	
	// 1. �߽����� ���Ѵ�.
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

	// 2. �߽��� ���� ��и����� ������

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

	// 3. ��и麰 ������ ����
	// 3-1. 1��и�: x�� ��������, ���� ������ y�� ��������
	// 3-2. 2��и�: x�� ��������, ���� ������ y�� ��������
	// 3-3. 3��и�: x�� ��������, ���� ������ y�� ��������
	// 3-4. 4��и�: x�� ��������, ���� ������ y�� ��������
	
	sort(quad1.begin(), quad1.end(), Object::CompareQuad1);
	sort(quad2.begin(), quad2.end(), Object::CompareQuad2);
	sort(quad3.begin(), quad3.end(), Object::CompareQuad3);
	sort(quad4.begin(), quad4.end(), Object::CompareQuad4);

	// 4. ���� �迭�� ��ģ��. copy�� ����
	quad1.insert(quad1.end(), quad2.begin(), quad2.end());
	quad1.insert(quad1.end(), quad3.begin(), quad3.end());
	quad1.insert(quad1.end(), quad4.begin(), quad4.end());
	copy(quad1.begin(), quad1.end(), vBuf);
	memcpy(objInfo.vBuffer, vBuf, sizeof(float) * objInfo.vCount * 3);


	// ���� �޸� ����
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

// ���ư��� ����� �Լ�

void Object::FlyingUpdate()
{
	deltaT = GET_SINGLE(TimeManager).GetDeltaTime();
	// ���ư���
	// parametric equation���� �Ķ���Ͱ� 1.0f�� �Ǹ� ����
	objInfo.flyParam += objInfo.flySpeed * deltaT;
	glm::vec3 dist = GetFlyingDistance(objInfo.flyParam);
	objInfo.flyMat = GET_SINGLE(TransformManager).GetTranslateMatrix(dist);

	// ȸ���ϱ�
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

// �������� ����� �Լ�

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

	// �ϴ±迡 �߽����� ������Ʈ
	GetCenter();
}

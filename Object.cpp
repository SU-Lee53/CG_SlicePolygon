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

	// 1. �߽����� ���Ѵ�. y���� �߽ɸ� ���ϸ� �ɵ���
	float middleY = 0.0;
	for (int i = 0; i < objInfo.vCount; i++)
	{
		middleY += vBuf[i][1];
	}
	middleY = middleY / objInfo.vCount;

	// 2. �߽������� ���κа� �Ʒ��κ��� ������
	vector<glm::vec3> upper;
	vector<glm::vec3> lower;
	
	for (int i = 0; i < objInfo.vCount; i++)
	{
		if (vBuf[i][1] >= middleY)
			upper.push_back(vBuf[i]);
		else
			lower.push_back(vBuf[i]);
	}

	// 3. ���κ��� x�� ��������, �Ʒ��κ��� ������������ �����Ѵ�.
	sort(upper.begin(), upper.end(), Object::CompareDescendingX);
	sort(lower.begin(), lower.end(), Object::CompareAscendingX);

	// 4. ���� �迭�� ��ģ��. copy�� ����
	upper.insert(upper.end(), lower.begin(), lower.end());
	copy(upper.begin(), upper.end(), vBuf);

	// ���� �޸� ����
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


// �������� ����� �Լ�

void Object::FlyingUpdate()
{
	// parametric equation���� �Ķ���Ͱ� 1.0f�� �Ǹ� ����

	// objInfo.state�� �������� ��Ȳ�̶�� ���ư��� �ӵ��� �ٿ��� 0���� ����
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

	// �ϴ±迡 �߽����� ������Ʈ
	GetCenter();
}

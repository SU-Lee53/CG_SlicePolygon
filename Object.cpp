#include "pch.h"
#include "Object.h"

Object::Object(glm::vec3* vertices, int vCount, RGB rgb)
{
	objInfo.objType = static_cast<OBJ_TYPE>(vCount);
	objInfo.objColor = rgb;
	objInfo.vCount = vCount;
	objInfo.pCount = vCount - 2;
	// �� ���縦 ���� �ӽ� ����
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
	// 1. ��ǥ�� �ݽð���� ����
	SortVertexCCW();
	
	// vec3�� ���� ������ vao�� �ֱ����� float�� �ٽ� ���� ����
	float temp[MAX_POINT * 3];		// ������ �ִ� ������ �̸� �Ҵ�
	float* vBuff = temp;
	memcpy(vBuff, vBuf, sizeof(float) * objInfo.vCount * 3);

	// 2. ���� ���� ����
	float cTemp[MAX_POINT * 3];		// ������ �ִ� ������ �̸� �Ҵ�
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
	unsigned int iTemp[(MAX_POINT - 2) * 3];		// ������ �ִ� ������ �̸� �Ҵ�
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

void Object::Render()
{
	glBindVertexArray(_vao->GetVAOHandle());
	glDrawElements(GL_TRIANGLES, _vao->GetElementCount(), GL_UNSIGNED_INT, 0);
}

#pragma once
#include <algorithm>

const int MAX_POINT = 30;

enum OBJ_TYPE
{
	OT_TRIANGLE = 3,
	OT_RECTANGLE = 4,
	OT_PENTAGON = 5,
	OT_NONE = 0
};

enum OBJ_STATUS
{
	OS_FLYING = 0,
	OS_FALLING,
	OS_OUT
};

struct OBJ_INFO
{
	OBJ_TYPE objType= OT_NONE;
	RGB objColor = { 0.0f, 0.0f, 0.0f };
	int vCount = 0;
	// pCount: �ش� object�� �����ϴ� �ﰢ��(������)�� ���� -> �ε��� ������ ũ��� pCount * 3
	// Ư���� ��찡 �ƴϸ� vCount-2 ?
	int pCount = 0;
	OBJ_STATUS state = OS_FLYING;
	glm::vec3 centerPos = glm::vec3(1.0f);
	
	// ���� ���۸� ���� ����. ������Ʈ �ɶ����� ������ġ ����
	float vBuffer[(MAX_POINT + 1) * 3];

	// ���ư��� ����� ������
	glm::vec3 startP = glm::vec3(1.0f);
	glm::vec3 endP = glm::vec3(1.0f);
	float flyparam = 0.0f;
	float flySpeed = 0.0001f;
	glm::mat4 flyMat = glm::mat4(1.0f);


	// �������� ����� ������
	float fallDist = 0.0f;
	float fallSpeed = (static_cast<float>(rand()) / RAND_MAX) * 0.0001;
	glm::mat4 fallMat = glm::mat4(1.0f);

	// ���� ��ȯ ���
	glm::mat4 finalMat = glm::mat4(1.0f);
};

class Object
{
public:
	Object(glm::vec3* vertices, int vCount, RGB rgb, glm::vec3 startP, float endY);
	~Object();

	// ������
	void Render();

	// ��ƿ
	OBJ_INFO GetObjInfo() { return objInfo; }
	glm::vec3* GetVertexBuf() { return vBuf; }
	VAO& GetVAO() { return *_vao; }
	void SetObjectStatus(OBJ_STATUS status) { objInfo.state = status; }
	
	// �̵�
	void FlyingUpdate();
	void GravityUpdate();

private:
	// ���� ���� ����
	void CreateFreeObject();

	// ���� ����
	void SortVertexCCW();
	static bool CompareAscendingX(const glm::vec3 a, const glm::vec3 b)
	{
		return a[0] < b[0];
	}
	static bool CompareDescendingX(const glm::vec3 a, const glm::vec3 b)
	{
		return a[0] > b[0];
	}

	void GetCenter();
	glm::vec3 GetFlyingDistance(float flyparam);

	void FinalMatUpdate();
	void vBufferUpdate();

private:

	VAO* _vao = nullptr;
	glm::vec3* vBuf;
	float* cBuf;
	unsigned int* idxBuf;

	OBJ_INFO objInfo;

};


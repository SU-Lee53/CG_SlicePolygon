#pragma once
#include <algorithm>

const int MAX_POINT = 6;

enum OBJ_TYPE
{
	OT_TRIANGLE = 3,
	OT_RECTANGLE = 4,
	OT_PENTAGON = 5,
	OT_NONE = 0
};

struct OBJ_INFO
{
	OBJ_TYPE objType= OT_NONE;
	RGB objColor = { 0.0f, 0.0f, 0.0f };
	int vCount = 0;
	// pCount: �ش� object�� �����ϴ� �ﰢ��(������)�� ���� -> �ε��� ������ ũ��� pCount * 3
	// Ư���� ��찡 �ƴϸ� vCount-2 ?
	int pCount = 0;
};

class Object
{
public:
	Object(glm::vec3* vertices, int vCount, RGB rgb);
	~Object();

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

	// ������
	void Render();

	// ��ƿ
	OBJ_INFO GetObjInfo() { return objInfo; }
	glm::vec3* GetVertexBuf() { return vBuf; }
	VAO& GetVAO() { return *_vao; }

private:

	VAO* _vao = nullptr;
	glm::vec3* vBuf;
	float* cBuf;
	unsigned int* idxBuf;

	OBJ_INFO objInfo;



};


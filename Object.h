#pragma once
#include <algorithm>
#include "Basket.h"

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
	OS_IN_BASKET,
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
	glm::vec3 flyDist = glm::vec3(1.0f);
	float flyParam = 0.0f;
	float flySpeed = 0.3f;
	glm::mat4 flyMat = glm::mat4(1.0f);

	// ���鼭 ���ư��� ����� ����
	float rotDeg = 0.0f;
	float rotSpeed = 100.0f;
	glm::mat4 rotMat = glm::mat4(1.0f);


	// �������� ����� ������
	float fallDist = 0.0f;
	float fallSpeed = (static_cast<float>(rand()) / (RAND_MAX / 7)) * 0.1;
	glm::mat4 fallMat = glm::mat4(1.0f);
	int moveDirection = 0;	// �߷����� �ش� �������� ��¦ �����̸� ������

	// �ٱ��Ͽ� ���������� ����� ������
	float yDist = 0.0f;
	glm::mat4 ibMat = glm::mat4(1.0f);

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
	void SetFlyDist(glm::vec3 dist) { objInfo.flyDist = dist; }
	void SetFallDist(float dist) { objInfo.fallDist = dist; }
	void SetMoveDirection(int dir) { objInfo.moveDirection = dir; }
	void SetFlyParam(float param) { objInfo.flyParam = param; }
	void SetInBasketYpos(float posY) { objInfo.yDist = posY; }
	// �̵�
	void FlyingUpdate();
	void GravityUpdate();
	void InBasketUpdate(Basket* basket);
	void vBufferUpdate(glm::mat4 matrix);

private:
	// ���� ���� ����
	void CreateFreeObject();

	// ���� ����
	void SortVertexCCW();
	static bool CompareQuad1(const glm::vec3 a, const glm::vec3 b)
	{
		if (a[0] > b[0] || ((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
		{
			if (((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
			{
				return a[1] < b[1];
			}
			else
			{
				return a[0] > b[0];
			}
		}
		return  a[0] > b[0];
	}
	
	static bool CompareQuad2(const glm::vec3 a, const glm::vec3 b)
	{
		if (a[0] > b[0] || ((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
		{
			if (((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
			{
				return a[1] > b[1];
			}
			else
			{
				return a[0] > b[0];
			}
		}
		return  a[0] > b[0];
	}

	static bool CompareQuad3(const glm::vec3 a, const glm::vec3 b)
	{
		if (a[0] < b[0] || ((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
		{
			if (((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
			{
				return a[1] > b[1];
			}
			else
			{
				return a[0] < b[0];
			}
		}
		return a[0] < b[0];
	}
	
	static bool CompareQuad4(const glm::vec3 a, const glm::vec3 b)
	{
		if (a[0] < b[0] || ((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
		{
			if (((a[0] - b[0] <= FLT_EPSILON) && (a[0] - b[0] >= 0)) || ((b[0] - a[0] <= FLT_EPSILON) && (b[0] - a[0] >= 0)))
			{
				return a[1] < b[1];
			}
			else
			{
				return a[0] < b[0];
			}
		}
		return a[0] < b[0];
	}

	void GetCenter();
	glm::vec3 GetFlyingDistance(float flyParam);

	void FinalMatUpdate();
private:

	VAO* _vao = nullptr;
	glm::vec3* vBuf;
	float* cBuf;
	unsigned int* idxBuf;

	OBJ_INFO objInfo;
	float deltaT = 0.0f;
};


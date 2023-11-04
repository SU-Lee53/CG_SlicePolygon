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
	// pCount: 해당 object를 구성하는 삼각형(폴리곤)의 갯수 -> 인덱스 버퍼의 크기는 pCount * 3
	// 특별한 경우가 아니면 vCount-2 ?
	int pCount = 0;
};

class Object
{
public:
	Object(glm::vec3* vertices, int vCount, RGB rgb);
	~Object();

	// 자유 도형 생성
	void CreateFreeObject();


	// 정점 정렬
	void SortVertexCCW();
	static bool CompareAscendingX(const glm::vec3 a, const glm::vec3 b)
	{
		return a[0] < b[0];
	}
	static bool CompareDescendingX(const glm::vec3 a, const glm::vec3 b)
	{
		return a[0] > b[0];
	}

	// 렌더링
	void Render();

	// 유틸
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


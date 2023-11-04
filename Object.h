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
	// pCount: 해당 object를 구성하는 삼각형(폴리곤)의 갯수 -> 인덱스 버퍼의 크기는 pCount * 3
	// 특별한 경우가 아니면 vCount-2 ?
	int pCount = 0;
	OBJ_STATUS state = OS_FLYING;
	glm::vec3 centerPos = glm::vec3(1.0f);
	
	// 정점 버퍼를 따로 저장. 업데이트 될때마다 정점위치 변경
	float vBuffer[(MAX_POINT + 1) * 3];

	// 날아갈때 사용할 변수들
	glm::vec3 startP = glm::vec3(1.0f);
	glm::vec3 endP = glm::vec3(1.0f);
	float flyparam = 0.0f;
	float flySpeed = 0.0001f;
	glm::mat4 flyMat = glm::mat4(1.0f);


	// 떨어질때 사용할 변수들
	float fallDist = 0.0f;
	float fallSpeed = (static_cast<float>(rand()) / RAND_MAX) * 0.0001;
	glm::mat4 fallMat = glm::mat4(1.0f);

	// 최종 변환 행렬
	glm::mat4 finalMat = glm::mat4(1.0f);
};

class Object
{
public:
	Object(glm::vec3* vertices, int vCount, RGB rgb, glm::vec3 startP, float endY);
	~Object();

	// 렌더링
	void Render();

	// 유틸
	OBJ_INFO GetObjInfo() { return objInfo; }
	glm::vec3* GetVertexBuf() { return vBuf; }
	VAO& GetVAO() { return *_vao; }
	void SetObjectStatus(OBJ_STATUS status) { objInfo.state = status; }
	
	// 이동
	void FlyingUpdate();
	void GravityUpdate();

private:
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


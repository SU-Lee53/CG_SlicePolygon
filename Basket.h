#pragma once
class Object;
using namespace std;

struct AABB
{
	float left;
	float top;
	float right;
	float bottom;
};

class Basket
{
public:
	Basket();
	~Basket();

	void Init();

	void Update(vector<Object*> objList);
	void Move();

	// 바구니에 닿은 조각을 찾아서 상태를 IN_BASKET으로 전환
	void GoalInUpdate(vector<Object*> objList);
	void BasketAABBUpdate();
	AABB GetObjectAABB(Object* obj);
	bool TouchedCheck(AABB p1, AABB p2);

	void Render();

	void vBufferUpdate();

	glm::mat4 GetBasketMat() { return moveMat; }
	float GetBasketMoveDist() { return moveDist; }
	float GetBasketHeight() { return height; }
private:
	Object* Shape;

	float height = -0.7f;
	float moveDist = 0.0f;
	float moveSpeed = 0.5f;
	bool direction = true;
	glm::mat4 moveMat = glm::mat4(1.0f);

	AABB basketAABB = { 0.0f, 0.0f, 0.0f ,0.0f };

};


#include "pch.h"
#include "Basket.h"

Basket::Basket()
{
	Init();
}

Basket::~Basket()
{
}

void Basket::Init()
{
	glm::vec3 vertices[] = {
		{0.4f, 0.05f, 0.0f},
		{-0.4f, 0.05f, 0.0f},
		{-0.4f, -0.05f, 0.0f},
		{0.4f, -0.05f, 0.0f}
	};
	glm::vec3* vp = vertices;
	RGB rgb = { 0.0f, 0.0f, 0.0f };

	Shape = new Object(vp, 4, rgb, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
}

void Basket::Update(vector<Object*> objList)
{
	Move();
	GoalInUpdate(objList);
}

void Basket::Move()
{
	float deltaT = GET_SINGLE(TimeManager).GetDeltaTime();

	if (direction)
	{
		// 오른쪽
		moveDist += moveSpeed * deltaT;
		moveMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(moveDist, height, 0.0f));
	}
	else
	{
		// 왼쪽
		moveDist -= moveSpeed * deltaT;
		moveMat = GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(moveDist, height, 0.0f));
	}

	if (moveDist >= 0.5f)
	{
		direction = false;
	}
	else if (moveDist <= -0.5f)
	{
		direction = true;
	}

	vBufferUpdate();
}

void Basket::GoalInUpdate(vector<Object*> objList)
{
	BasketAABBUpdate();

	for (int i = 0; i < objList.size(); i++)
	{
		if (objList.at(i)->GetObjInfo().state == OS_IN_BASKET || objList.at(i)->GetObjInfo().state == OS_FLYING)
			continue;

		AABB objAABB = GetObjectAABB(objList.at(i));
		if (TouchedCheck(basketAABB, objAABB) == true)
		{
			objList.at(i)->SetObjectStatus(OS_IN_BASKET);
			objList.at(i)->SetInBasketYpos(objList.at(i)->GetObjInfo().fallDist);

			cout << objList.at(i)->GetObjInfo().fallDist << endl;
		}
	}
}

void Basket::BasketAABBUpdate()
{
	basketAABB = {
		Shape->GetObjInfo().centerPos[0] - 0.4f,
		Shape->GetObjInfo().centerPos[1] + 0.05f,
		Shape->GetObjInfo().centerPos[0] + 0.4f,
		Shape->GetObjInfo().centerPos[1] - 0.05f,
	};
}

AABB Basket::GetObjectAABB(Object* obj)
{
	float minX = FLT_MAX;
	float maxX = FLT_MIN;
	float minY = FLT_MAX;
	float maxY = FLT_MIN;

	for (int i = 0; i < obj->GetObjInfo().vCount; i++)
	{
		int idx = i * 3;
		// x값 최대, 최소
		if (obj->GetObjInfo().vBuffer[idx] < minX)
		{
			minX = obj->GetObjInfo().vBuffer[idx];
		}
		if (obj->GetObjInfo().vBuffer[idx] > maxX)
		{
			maxX = obj->GetObjInfo().vBuffer[idx];
		}

		// y값 최대 최소
		if (obj->GetObjInfo().vBuffer[idx + 1] < minY)
		{
			minY = obj->GetObjInfo().vBuffer[idx + 1];
		}
		if (obj->GetObjInfo().vBuffer[idx + 1] > maxY)
		{
			maxY = obj->GetObjInfo().vBuffer[idx + 1];
		}
	}

	AABB ret = { minX, maxY, maxX, minY };

	return ret;

}

bool Basket::TouchedCheck(AABB p1, AABB p2)
{
	if (p1.right > p2.left && p2.right > p1.left)
	{
		if (p2.bottom - p1.top <= 0.005f && p2.bottom - p1.top >= 0)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void Basket::Render()
{
	Shape->Render();
}

void Basket::vBufferUpdate()
{
	Shape->vBufferUpdate(moveMat);

}
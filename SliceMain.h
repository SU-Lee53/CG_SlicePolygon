#pragma once

struct LINE_INFO
{
	Coord start;
	Coord end;
};

class SliceMain
{
public:
	SliceMain();
	~SliceMain();

	void init();
	void drawScene();
	void Reshape(int w, int h);

	void MouseUpdate();
	void MotionUpdate();
	void KeyboardUpdate();

	void Render();
	void MoveUpdate();

	void CheckOutofScreen();
	void RemoveObject();

	void MakeObjectByTime();
private:	
	Camera* cam;
	Projection* proj;
	glm::mat4 worldMat = glm::mat4(1.0f);
	unsigned int shaderID;

	LINE_INFO sliceLine = { {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f} };

	vector<Object*> objList;

	time_t prevTime = time(nullptr);

};


#pragma once
#include "Slicetool.h"

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

	void Render();


private:
	LINE_INFO sliceLine = { {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f} };



};


#include "pch.h"
#include "SliceMain.h"

SliceMain::SliceMain()
{
	init();
}

SliceMain::~SliceMain()
{
}

void SliceMain::init()
{
}

void SliceMain::drawScene()
{
	glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	MouseUpdate();
	MotionUpdate();

	Render();

	glutSwapBuffers();
	glutPostRedisplay();
}

void SliceMain::Reshape(int w, int h)
{
}

void SliceMain::MouseUpdate()
{
	MouseState* ms = GET_SINGLE(InputManager).DequeueMouseBuf();
	if (ms == nullptr)
	{
		return;
	}

	if (ms->button == GLUT_LEFT && ms->state == GLUT_DOWN)
	{
		sliceLine.end.x = -1.0f;
		sliceLine.start.x = ms->pos.x;
		sliceLine.start.y = ms->pos.y;

	}
	else if (ms->button == GLUT_LEFT && ms->state == GLUT_UP)
	{
		sliceLine.end.x = ms->pos.x;
		sliceLine.end.y = ms->pos.y;
	}

	delete ms;
}

void SliceMain::MotionUpdate()
{
	Coord* ms = GET_SINGLE(InputManager).DequeueMotionBuf();
	if (ms == nullptr)
	{
		return;
	}

	sliceLine.end.x = ms->x;
	sliceLine.end.y = ms->y;

	delete ms;
}

void SliceMain::Render()
{
	if(sliceLine.end.x != -1.0f)
		DrawLine(sliceLine.start, sliceLine.end);

}



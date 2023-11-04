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
	srand(time(0));
	cam = new Camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.0f, 0.f));
	proj = new Projection(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
}

void SliceMain::drawScene()
{
	shaderID = GET_SINGLE(Core).GetShaderID();
	glUseProgram(shaderID);
	cam->Bind(shaderID);
	proj->Bind(shaderID);
	GET_SINGLE(TransformManager).Bind(worldMat, shaderID);


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
		GET_SINGLE(Slicetool).Slice(sliceLine, obj);
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
	if (obj == nullptr)
	{
		glm::vec3 vertices[] = {
			// glm::vec3(0.3f, 0.5f, 0.0f),
			// glm::vec3(-0.3f, 0.5f, 0.0f),
			glm::vec3(0.0f, 0.5f, 0.0f),
			glm::vec3(-0.5f, 0.0f, 0.0f),
			glm::vec3(0.5f, 0.0f, 0.0f),
			glm::vec3(-0.3f, -0.5f, 0.0f),
			glm::vec3(0.3f, -0.5f, 0.0f)
		};
		RGB rgb = { 0.3f, 0.4f, 0.5f };
		obj = new Object(vertices, 5, rgb);

	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	obj->Render();


	if (sliceLine.end.x != -1.0f)
		DrawLine(sliceLine.start, sliceLine.end);

	// 현재해야할일
	// 1. 정점만으로 Object 만드는 클래스 구현
	// 2. 이를 위한 정점 반시계 정렬 구현 -> 중심점을 만들고 사분면별로 가져와서 버퍼에 넣으면?
}



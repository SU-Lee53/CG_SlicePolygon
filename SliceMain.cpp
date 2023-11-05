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
	KeyboardUpdate();
	if(objList.size() != 0)
	{
		MoveUpdate();

		CheckOutofScreen();
		RemoveObject();
	}


	MakeObjectByTime();
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
		sliceLine.end.y = -1.0f;
		sliceLine.start.x = ms->pos.x;
		sliceLine.start.y = ms->pos.y;
	}
	else if (ms->button == GLUT_LEFT && ms->state == GLUT_UP)
	{
		sliceLine.end.x = ms->pos.x;
		sliceLine.end.y = ms->pos.y;
		vector<Object*> newObjVector;
		for(int i = objList.size() - 1; i >= 0; i--)
		{
			if (GET_SINGLE(Slicetool).IntersectCheck(sliceLine, objList.at(i)))
			{
				Object* newObj[2];
				GET_SINGLE(Slicetool).Slice(sliceLine, objList.at(i), newObj);
				newObjVector.push_back(newObj[0]);
				newObjVector.push_back(newObj[1]);
				objList.erase(objList.begin() + i);
			}
		}
		objList.insert(objList.end(), newObjVector.begin(), newObjVector.end());
		sliceLine.start.x = -1.0f;
		sliceLine.start.y = -1.0f;
		sliceLine.end.x = -1.0f;
		sliceLine.end.y = -1.0f;


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

void SliceMain::KeyboardUpdate()
{
	KeyboardState* ks = GET_SINGLE(InputManager).DequeueKeyboardBuf();
	if (ks == nullptr)
	{
		return;
	}

	switch (ks->key)
	{
	case 'm':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case 'M':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

	}

}

void SliceMain::Render()
{
	GET_SINGLE(TimeManager).SetPrevFrameTime();

	if (objList.size() != 0)
	{
		for (int i = 0; i < objList.size(); i++)
		{
			GET_SINGLE(TransformManager).Bind(worldMat * objList.at(i)->GetObjInfo().finalMat, shaderID);
			objList.at(i)->Render();

		}
	}

	GET_SINGLE(TransformManager).Bind(worldMat, shaderID);

	if (sliceLine.start.x != -1.0f && sliceLine.start.y != -1.0f && sliceLine.end.x != -1.0f && sliceLine.end.y != -1.0f)
	{
		GET_SINGLE(TransformManager).Bind(worldMat, shaderID);
		DrawLine(sliceLine.start, sliceLine.end);
	}

	
}

void SliceMain::MoveUpdate()
{
	for (int i = 0; i < objList.size(); i++)
	{
		if (objList.at(i)->GetObjInfo().state == OS_FALLING)
		{
			objList.at(i)->GravityUpdate();
		}
		else
			objList.at(i)->FlyingUpdate();
	}


}

void SliceMain::CheckOutofScreen()
{
	for (int i = 0; i < objList.size(); i++)
	{
		float lparam, ydist;
		lparam = objList.at(i)->GetObjInfo().flyParam;
		ydist = objList.at(i)->GetObjInfo().fallDist;
		if (lparam >= 1.0f || ydist <= -2.0f)
		{
			objList.at(i)->SetObjectStatus(OS_OUT);
		}
	}
}

void SliceMain::RemoveObject()
{
	for (int i = objList.size() - 1; i >= 0; i--)
	{
		if (objList.at(i)->GetObjInfo().state == OS_OUT)
		{
			objList.erase(objList.begin() + i);
		}
	}
}

void SliceMain::MakeObjectByTime()
{
	time_t currentTime = time(nullptr);
	
	// 1초에 1번씩
	if (currentTime - prevTime >= 2)
	{
		int objType = rand() % 3 + 4;
		glm::vec3* vtx = nullptr;

		switch (objType)
		{
		case 3:
		{
			glm::vec3 temp[] = {
				glm::vec3(0.0f, 0.2f, 0.0f),
				glm::vec3(0.2f, -0.2f, 0.0f),
				glm::vec3(-0.2, -0.2f, 0.0f),
			};
			vtx = temp;
			break;
		}

		case 4:
		{
			glm::vec3 temp[] = {
				glm::vec3(0.2f, 0.2f, 0.0f),
				glm::vec3(-0.2f, 0.2f, 0.0f),
				glm::vec3(0.2f, -0.2f, 0.0f),
				glm::vec3(-0.2, -0.2f, 0.0f),
			};
			vtx = temp;
			break;
		}

		case 5:
		{
			glm::vec3 temp[] = {
				glm::vec3(0.0f, 0.2f, 0.0f),
				glm::vec3(0.2f, 0.05f, 0.0f),
				glm::vec3(-0.2f, 0.05f, 0.0f),
				glm::vec3(-0.13f, -0.2f, 0.0f),
				glm::vec3(0.13f, -0.2f, 0.0f)
			};
			vtx = temp;
			break;
		}

		case 6:
		{
			glm::vec3 temp[] = {
				glm::vec3(0.1f, 0.2f, 0.0f),
				glm::vec3(-0.1f, 0.2f, 0.0f),
				glm::vec3(0.2f, 0.0f, 0.0f),
				glm::vec3(-0.2f, 0.0f, 0.0f),
				glm::vec3(0.1f, -0.2f, 0.0f),
				glm::vec3(-0.1, -0.2f, 0.0f),
			};
			vtx = temp;
		}
		}
		RGB rgb = GetRandomRGB();
		glm::vec3 startP = glm::vec3(-1.5f, static_cast<float>(rand()) / RAND_MAX, 0.0f);
		float endY = static_cast<float>(rand()) / RAND_MAX;
		objList.push_back(new Object(vtx, objType, rgb, startP, endY));
		prevTime = currentTime;
	}


}



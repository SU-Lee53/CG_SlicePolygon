#include "pch.h"
#include "SliceMain.h"

LOAD_EX(SliceMain);

void drawScene()
{
	ex->drawScene();
}

void Reshape(int w, int h)
{
	ex->Reshape(w, h);
}

int main(int argc, char** argv)
{
	Core::GetInstance().InitScreen(argc, argv, 100, 100, 800, 800, "Examples");
	Core::GetInstance().InitShader();

	Core::GetInstance().BindDrawCallback(drawScene);
	Core::GetInstance().BindReshapeCallback(Reshape);
	Core::GetInstance().UseKeyboardCallback();
	Core::GetInstance().UseMouseCallback();
	Core::GetInstance().UseMotionCallback();
	Core::GetInstance().UseSpecialKeyCallback();
	

	while (true)
	{
		glutMainLoopEvent();
	}

}

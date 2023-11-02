#include "pch.h"
#include "Core.h"

Core& Core::GetInstance()
{
	static Core s;
	return s;

}

Core::Core()
{
}

Core::~Core()
{
	delete shader;
}

void Core::InitScreen(int argc, char** argv, int position_x, int position_y, int width, int height, string windowName)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(position_x, position_y);
	glutInitWindowSize(width, height);
	glutCreateWindow(windowName.c_str());

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW 초기화 실패" << endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "GLEW 초기화 성공" << endl;

}

void Core::InitShader()
{
	Core::GetInstance().shader = new Shaders();
}

void Core::BindDrawCallback(void (*funcptr)())
{
	glutDisplayFunc(funcptr);
}

void Core::BindReshapeCallback(void (*funcptr)(int w, int h))
{
	glutReshapeFunc(funcptr);
}

void Core::UseMouseCallback()
{
	glutMouseFunc(InputManager::GetInstance().MouseFunc());
}

void Core::UseMotionCallback()
{
	glutMotionFunc(InputManager::GetInstance().MotionFunc());
}

void Core::UseKeyboardCallback()
{
	glutKeyboardFunc(InputManager::GetInstance().KeyboardFunc());
}

void Core::UseSpecialKeyCallback()
{
	glutSpecialFunc(InputManager::GetInstance().SpecialKeyFunc());
}




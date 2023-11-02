#pragma once
#include <string>
using namespace std;


class Core
{
public:

	static Core& GetInstance();
	
	void InitScreen(int argc, char** argv, int position_x, int position_y, int width, int height, string windowName);
	void InitShader();
	/* drawScene과 Reshape를 만들어서 여기다 바인드하면 콜백함수가 호출됩니다.*/
	void BindDrawCallback(void (*funcptr)());
	void BindReshapeCallback(void (*funcptr)(int w, int h));

	/* 아래의 4가지 함수는 각 콜백을 사용하려고 할때 호출하면 됩니다.*/
	void UseMouseCallback();
	void UseMotionCallback();
	void UseKeyboardCallback();
	void UseSpecialKeyCallback();

	int GetShaderID() { return shader->GetShaderID(); }

private:
	Core();
	~Core();

private:
	Shaders* shader;
};

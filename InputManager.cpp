#include "pch.h"
#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::MouseInput(int button, int state, int x, int y)
{
	Coord pos = { x, y };
	pos = AdjustMouseCoordinate(pos);
	MouseState* s = new MouseState { button, state, pos };
	InputManager::GetInstance().GetMouseBuf().push(s);
	glutPostRedisplay();
}

void InputManager::MotionInput(int x, int y)
{
	Coord pos = AdjustMouseCoordinate({static_cast<float>(x), static_cast<float>(y) });
	Coord* s = new Coord{ pos.x, pos.y };
	InputManager::GetInstance().GetMotionBuf().push(s);
	glutPostRedisplay();
}

void InputManager::KeyboardInput(unsigned char key, int x, int y)
{
	Coord pos = { x, y };
	pos = AdjustMouseCoordinate(pos);
	KeyboardState* s = new KeyboardState{ key, pos };
	InputManager::GetInstance().GetKeyboardBuf().push(s);
	glutPostRedisplay();
}

void InputManager::SpecialKeyInput(int key, int x, int y)
{
	Coord pos = { x, y };
	pos = AdjustMouseCoordinate(pos);
	SpecialState* s = new SpecialState{ key, pos };
	InputManager::GetInstance().GetSpecialKeyBuf().push(s);
	glutPostRedisplay();
}

MouseState* InputManager::DequeueMouseBuf()
{
	if (mouseBuf.empty())
	{
		return nullptr;
	}
	MouseState* temp = mouseBuf.front();

	// 반환을 위해 새로 할당. 실습객체에서 사용후 반드시 delete 할것
	MouseState* ret = new MouseState{temp->button, temp->state, temp->pos };
	mouseBuf.pop();
	return ret;
}

Coord* InputManager::DequeueMotionBuf()
{
	if (keyboardBuf.empty())
	{
		return nullptr;
	}

	Coord* temp = motionBuf.front();

	// 반환을 위해 새로 할당. 실습객체에서 사용후 반드시 delete 할것
	Coord* ret = new Coord{ temp->x, temp->y, temp->z };
	motionBuf.pop();
	return ret;
}

KeyboardState* InputManager::DequeueKeyboardBuf()
{
	if (keyboardBuf.empty())
	{
		return nullptr;
	}

	KeyboardState* temp = keyboardBuf.front();
	KeyboardState* ret = new KeyboardState{ temp->key, temp->pos };
	keyboardBuf.pop();
	return ret;
}

SpecialState* InputManager::DequeueSpecialKeyBuf()
{
	if (specialKeyBuf.empty())
	{
		return nullptr;
	}

	SpecialState* temp = specialKeyBuf.front();
	SpecialState* ret = new SpecialState{ temp->key, temp->pos };
	specialKeyBuf.pop();
	return ret;
}

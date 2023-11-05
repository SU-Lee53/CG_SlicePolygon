#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Utills.h"

Coord AdjustMouseCoordinate(Coord coord)
{
	GLint width = glutGet(GLUT_WINDOW_WIDTH);
	GLint height = glutGet(GLUT_WINDOW_HEIGHT);

	coord.x = (coord.x - (width / 2)) / (width / 2);
	coord.y = -1 * (coord.y - (height / 2)) / (height / 2);

	return coord;
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

Coord GetRandomCoord()
{
	Coord pos;
	pos.x = static_cast<float>(rand()) / (RAND_MAX / 2) - 1.0f;
	pos.y = static_cast<float>(rand()) / (RAND_MAX / 2) - 1.0f;
	pos.z = static_cast<float>(rand()) / (RAND_MAX / 2) - 1.0f;

	return pos;
}

RGB GetRandomRGB()
{
	RGB rgb;
	rgb.Red = static_cast<float>(rand()) / RAND_MAX;
	rgb.Green = static_cast<float>(rand()) / RAND_MAX;
	rgb.Blue = static_cast<float>(rand()) / RAND_MAX;

	return rgb;
}

void DrawAxis(float length)
{
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-length, 0.0, 0.0);
	glVertex3f(length, 0.0, 0.0);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0, -length, 0.0);
	glVertex3f(0.0, length, 0.0);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0, 0.0, -length);
	glVertex3f(0.0, 0.0, length);
	glEnd();
}

void DrawLine(Coord start, Coord end)
{
	glBegin(GL_LINES);
	
	glVertex3f(start.x, start.y, 0.0);
	glVertex3f(end.x, end.y, 0.0);

	glEnd();
}

void DrawPoint(Coord point)
{
	glPointSize(10.0f);
	glBegin(GL_POINTS);

	glVertex3f(point.x, point.y, 0.0);

	glEnd();
}

void SetBackground(RGB rgb)
{
	glClearColor(rgb.Red, rgb.Green, rgb.Blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

glm::vec3 GetRandomVec3()
{
	glm::vec3 temp;
	temp.x = static_cast<float>(rand()) / (RAND_MAX / 2) - 1.0f;
	temp.y = static_cast<float>(rand()) / (RAND_MAX / 2) - 1.0f;
	temp.z = static_cast<float>(rand()) / (RAND_MAX / 2) - 1.0f;

	return temp;
}
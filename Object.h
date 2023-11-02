#pragma once

enum OBJ_TYPE
{
	LINE = 0,
	TRIANGLE,
	RECTANGLE,
	PENTAGON,
	HEXAGON,
	NONE
};

struct OBJ_INFO
{
	OBJ_TYPE objType= NONE;
};

class Object
{
public:
	Object();
	~Object();




private:
	VAO* _vao = nullptr;

};


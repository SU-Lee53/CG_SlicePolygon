#pragma once

class Shaders
{

public:
	Shaders();
	~Shaders();

	// 셰이더 컴파일 함수
	GLuint makeShaderProgram();
	// 정점 버퍼 함수

	int GetShaderID() { return shaderID; }

private:
	void makeVertexShaders();
	void makeFragmentShaders();

private:
	GLuint shaderID;

	GLuint vertexShader;
	GLuint fragmentShader;
	
};


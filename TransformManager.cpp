#include "pch.h"
#include "TransformManager.h"


TransformManager::TransformManager()
{
}

TransformManager::~TransformManager()
{
}

glm::mat4 TransformManager::GetTranslateMatrix(glm::vec3 vec)
{
	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, vec);

	return translate;
}

glm::mat4 TransformManager::GetRotateMatrix(float degree, AXIS axis)
{
	glm::mat4 rotate = glm::mat4(1.0f);
	glm::vec3 vAxis = glm::vec3(0.0f, 0.0f, 0.0f);

	vAxis[static_cast<int>(axis)] = 1.0f;

	rotate = glm::rotate(rotate, glm::radians(degree), vAxis);

	return rotate;
}

glm::mat4 TransformManager::GetRotateMatrix(float degree, glm::vec3 axis)
{
	glm::mat4 rotate = glm::mat4(1.0f);
	rotate = glm::rotate(rotate, glm::radians(degree), axis);

	return rotate;
}

glm::mat4 TransformManager::GetScaleMatrix(glm::vec3 vec)
{
	glm::mat4 scale = glm::mat4(1.0f);
	scale = glm::scale(scale, vec);

	return scale;
}

void TransformManager::Bind(glm::mat4 matrix, unsigned int shaderID)
{
	glUseProgram(shaderID);
	unsigned int uniformLocation = glGetUniformLocation(shaderID, "model");
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

#include "pch.h"
#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 EYE, glm::vec3 AT, glm::vec3 UP) : _EYE(EYE), _AT(AT), _UP(UP)
{
	SetCamera();
	EYE2ATDistance = glm::sqrt(powf(AT[0] - EYE[0], 2) + powf(AT[2] - EYE[2], 2));
}

Camera::~Camera()
{
}

void Camera::CameraMove(glm::vec3 move)
{
	// _cameraMatrix = _cameraMatrix * GET_SINGLE(TransformManager).GetTranslateMatrix(move);

	_EYE = _EYE + move;
	_AT = _AT + move;
	_cameraMatrix = glm::lookAt(_EYE, _AT, _UP);
}

void Camera::CameraRotation(float degree, AXIS axis)
{
	_AT[0] = EYE2ATDistance * glm::sin(glm::radians(degree)) + _EYE[0];
	_AT[2] = EYE2ATDistance * glm::cos(glm::radians(degree)) + _EYE[2];
	_cameraMatrix = glm::lookAt(_EYE, _AT, _UP);

}

void Camera::CameraRevolution(float degree, AXIS axis)
{
	_EYE[0] = EYE2ATDistance * glm::sin(glm::radians(degree)) + _AT[0];
	_EYE[2] = EYE2ATDistance * glm::cos(glm::radians(degree)) + _AT[2];
	_cameraMatrix = glm::lookAt(_EYE, _AT, _UP);

	// _cameraMatrix = _cameraMatrix * GET_SINGLE(TransformManager).GetRotateMatrix(degree, axis);
}


void Camera::SetCamera()
{
	_cameraMatrix = glm::lookAt(_EYE, _AT, _UP);

	// lookAt의 결과로 나오는 뷰 변환 행렬
	// 
	//	|	_u(x)	_u(y)	_u(z)	-_EYE·_u	|
	//	|										|
	//	|	_v(x)	_v(y)	_v(z)	-_EYE·_v	|
	//	|										|
	//	|	_n(x)	_n(y)	_n(z)	-_EYE·_n	|
	//	|										|
	//	|	  0	      0	      0			1		|
}

void Camera::SetWorldMat(glm::mat4 worldMat)
{
	_worldMat = worldMat;
	
}

void Camera::ResetCamera(glm::vec3 EYE, glm::vec3 AT, glm::vec3 UP)
{
	_EYE = EYE;
	_AT = AT;
	_UP = UP;
	_cameraMatrix = glm::lookAt(_EYE, _AT, _UP);
}

void Camera::Bind(unsigned int shaderID)
{
	unsigned int viewLocation = glGetUniformLocation(shaderID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &_cameraMatrix[0][0]);
}
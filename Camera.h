#pragma once
class Camera
{
public:
	Camera(glm::vec3 EYE, glm::vec3 AT, glm::vec3 UP);
	~Camera();

	void CameraMove(glm::vec3 move);

	// 밑에 2개 지금 문제있음
	void CameraRotation(float degree, AXIS axis);
	void CameraRevolution(float degree, AXIS axis);

	glm::mat4 GetCameraMatrix() { return _cameraMatrix; }
	void Bind(unsigned int shaderID);

	void SetWorldMat(glm::mat4 worldMat);
	void ResetCamera(glm::vec3 EYE, glm::vec3 AT, glm::vec3 UP);

private:
	void SetCamera();
	

	


private:
	// 월드 공간에서의 카메라의 위치
	glm::vec3 _EYE;
	// 월드 공간에서 카메라가 바라보는 기준점
	glm::vec3 _AT;
	// 카메라의 상단이 가리키는 방향
	glm::vec3 _UP;

	glm::mat4 _cameraMatrix;

	glm::mat4 _worldMat = glm::mat4(1.0f);

	float EYE2ATDistance = 0.0f;
};


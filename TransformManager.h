#pragma once

class TransformManager
{
public:
	static TransformManager& GetInstance()
	{
		static TransformManager instance;
		return instance;
	}
	
	glm::mat4 GetTranslateMatrix(glm::vec3 move);

	// x,y,z,축을 회전축으로 이용할때 사용합니다.
	glm::mat4 GetRotateMatrix(float degree, AXIS axis);

	// 임의의 회전축을 이용할때 사용합니다.
	glm::mat4 GetRotateMatrix(float degree, glm::vec3 axis);

	glm::mat4 GetScaleMatrix(glm::vec3 scale);

	void Bind(glm::mat4 matrix, unsigned int shaderID);

private:
	TransformManager();
	~TransformManager();

};


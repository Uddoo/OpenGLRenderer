#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
	                 GLfloat red, GLfloat green, GLfloat blue,
	                 GLfloat aIntensity, GLfloat dIntensity,
	                 GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	              GLfloat diffuseIntensityLocation, GLfloat directionLocation); // 使用光源

	glm::mat4 CalculateLightTransform(); // 计算光源变换矩阵

	~DirectionalLight();

private:
	glm::vec3 direction; // 光源方向
};

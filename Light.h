#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation); // 使用光源

	~Light();

private:
	glm::vec3 colour; // 光源颜色
	GLfloat ambientIntensity; // 环境光强度

	glm::vec3 direction; // 光源方向
	GLfloat diffuseIntensity; // 漫反射强度
};

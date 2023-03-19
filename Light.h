#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity);

	~Light();

protected:
	glm::vec3 colour; // 光源颜色
	GLfloat ambientIntensity; // 环境光强度


	GLfloat diffuseIntensity; // 漫反射强度
};

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
	glm::vec3 colour; // ��Դ��ɫ
	GLfloat ambientIntensity; // ������ǿ��


	GLfloat diffuseIntensity; // ������ǿ��
};

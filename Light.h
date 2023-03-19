#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation); // ʹ�ù�Դ

	~Light();

private:
	glm::vec3 colour; // ��Դ��ɫ
	GLfloat ambientIntensity; // ������ǿ��

	glm::vec3 direction; // ��Դ����
	GLfloat diffuseIntensity; // ������ǿ��
};

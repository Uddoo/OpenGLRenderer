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
		GLuint diffuseIntensityLocation, GLuint directionLocation); // ʹ�ù�Դ

	~Light();

private:
	glm::vec3 colour; // ��Դ��ɫ
	GLfloat ambientIntensity; // ������ǿ��

	glm::vec3 direction; // ��Դ����
	GLfloat diffuseIntensity; // ������ǿ��
};

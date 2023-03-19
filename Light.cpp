#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // ������Ĭ�ϰ�ɫ
	ambientIntensity = 1.0f; // ������Ĭ��ǿ��Ϊ1

	diffuseIntensity = 0.0f; // ������Ĭ��ǿ��Ϊ0
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	diffuseIntensity = dIntensity; // ������ǿ��
}

Light::~Light()
{
}
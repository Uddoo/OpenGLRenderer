#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // 环境光默认白色
	ambientIntensity = 1.0f; // 环境光默认强度为1

	diffuseIntensity = 0.0f; // 漫反射默认强度为0
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	diffuseIntensity = dIntensity; // 漫反射强度
}

Light::~Light()
{
}
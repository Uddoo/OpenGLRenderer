#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // 环境光默认白色
	ambientIntensity = 1.0f; // 环境光默认强度为1

	direction = glm::vec3(0.0f, -1.0f, 0.0f); // 漫反射方向
	diffuseIntensity = 0.0f; // 漫反射默认强度为0
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir); // 漫反射方向
	diffuseIntensity = dIntensity; // 漫反射强度
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z); // 设置环境光颜色
	glUniform1f(ambientIntensityLocation, ambientIntensity); // 设置环境光强度

	glUniform3f(directionLocation, direction.x, direction.y, direction.z); // 设置漫反射方向
	glUniform1f(diffuseIntensityLocation, diffuseIntensity); // 设置漫反射强度
}

Light::~Light()
{
}
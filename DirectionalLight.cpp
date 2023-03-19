#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f); // 漫反射方向
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir); // 漫反射方向
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z); // 设置环境光颜色
	glUniform1f(ambientIntensityLocation, ambientIntensity); // 设置环境光强度

	glUniform3f(directionLocation, direction.x, direction.y, direction.z); // 设置漫反射方向
	glUniform1f(diffuseIntensityLocation, diffuseIntensity); // 设置漫反射强度
}

DirectionalLight::~DirectionalLight()
{
}

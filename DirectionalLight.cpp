#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f); // 漫反射方向
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f); // 设置光源投影矩阵
}

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
                                   GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat aIntensity, GLfloat dIntensity,
                                   GLfloat xDir, GLfloat yDir, GLfloat zDir)
	: Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir); // 漫反射方向
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f); // 设置光源投影矩阵
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
                                GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z); // 设置环境光颜色
	glUniform1f(ambientIntensityLocation, ambientIntensity); // 设置环境光强度

	glUniform3f(directionLocation, direction.x, direction.y, direction.z); // 设置漫反射方向
	glUniform1f(diffuseIntensityLocation, diffuseIntensity); // 设置漫反射强度
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * lookAt(-direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 计算光源变换矩阵
}

DirectionalLight::~DirectionalLight()
{
}

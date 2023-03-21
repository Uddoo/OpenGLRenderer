#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLfloat con, GLfloat lin, GLfloat exp,
	GLfloat edge) : PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	this->edge = edge;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z); // 设置环境光颜色
	glUniform1f(ambientIntensityLocation, ambientIntensity); // 设置环境光强度

	glUniform1f(diffuseIntensityLocation, diffuseIntensity); // 设置漫反射强度

	glUniform3f(positionLocation, position.x, position.y, position.z); // 设置点光源位置
	glUniform1f(constantLocation, constant); // 设置点光源常数
	glUniform1f(linearLocation, linear); // 设置点光源线性
	glUniform1f(exponentLocation, exponent); // 设置点光源指数

	glUniform3f(directionLocation, direction.x, direction.y, direction.z); // 设置聚光灯方向
	glUniform1f(edgeLocation, procEdge); // 设置聚光灯边缘
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}

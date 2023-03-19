#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; // 因为 exponent * x ^ 2 + linear  * x + constant 处于分母，不能为0，constant不受距离影响
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z); // 设置环境光颜色
	glUniform1f(ambientIntensityLocation, ambientIntensity); // 设置环境光强度

	glUniform1f(diffuseIntensityLocation, diffuseIntensity); // 设置漫反射强度

	glUniform3f(positionLocation, position.x, position.y, position.z); // 设置点光源位置
	glUniform1f(constantLocation, constant); // 设置点光源常数
	glUniform1f(linearLocation, linear); // 设置点光源线性
	glUniform1f(exponentLocation, exponent); // 设置点光源指数
}

PointLight::~PointLight()
{
}

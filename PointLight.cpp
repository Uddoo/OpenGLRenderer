#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; // ��Ϊ exponent * x ^ 2 + linear  * x + constant ���ڷ�ĸ������Ϊ0��constant���ܾ���Ӱ��
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
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z); // ���û�������ɫ
	glUniform1f(ambientIntensityLocation, ambientIntensity); // ���û�����ǿ��

	glUniform1f(diffuseIntensityLocation, diffuseIntensity); // ����������ǿ��

	glUniform3f(positionLocation, position.x, position.y, position.z); // ���õ��Դλ��
	glUniform1f(constantLocation, constant); // ���õ��Դ����
	glUniform1f(linearLocation, linear); // ���õ��Դ����
	glUniform1f(exponentLocation, exponent); // ���õ��Դָ��
}

PointLight::~PointLight()
{
}

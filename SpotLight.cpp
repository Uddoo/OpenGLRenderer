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
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z); // ���û�������ɫ
	glUniform1f(ambientIntensityLocation, ambientIntensity); // ���û�����ǿ��

	glUniform1f(diffuseIntensityLocation, diffuseIntensity); // ����������ǿ��

	glUniform3f(positionLocation, position.x, position.y, position.z); // ���õ��Դλ��
	glUniform1f(constantLocation, constant); // ���õ��Դ����
	glUniform1f(linearLocation, linear); // ���õ��Դ����
	glUniform1f(exponentLocation, exponent); // ���õ��Դָ��

	glUniform3f(directionLocation, direction.x, direction.y, direction.z); // ���þ۹�Ʒ���
	glUniform1f(edgeLocation, procEdge); // ���þ۹�Ʊ�Ե
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}

#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine); // sIntensity = specular intensity, shine = shininess

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation); // ʹ�ò���

	~Material();

private:
	GLfloat specularIntensity; // �����ǿ��
	GLfloat shininess; // �߹�ָ��
};


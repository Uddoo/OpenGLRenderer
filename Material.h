#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine); // sIntensity = specular intensity, shine = shininess

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation); // 使用材质

	~Material();

private:
	GLfloat specularIntensity; // 镜面光强度
	GLfloat shininess; // 高光指数
};


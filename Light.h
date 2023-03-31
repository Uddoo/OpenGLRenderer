#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight,
	      GLfloat red, GLfloat green, GLfloat blue,
	      GLfloat aIntensity, GLfloat dIntensity);

	// 获取阴影贴图
	ShadowMap* getShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 colour; // 光源颜色
	GLfloat ambientIntensity; // 环境光强度

	GLfloat diffuseIntensity; // 漫反射强度

	glm::mat4 lightProj; // 光源投影矩阵

	ShadowMap* shadowMap; // 阴影贴图
};

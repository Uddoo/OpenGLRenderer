#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(GLsizei width, GLsizei height); // 初始化阴影贴图

	virtual void Write(); // 开始写入阴影贴图

	virtual void Read(GLenum textureUnit); // 读取阴影贴图

	GLsizei GetShadowWidth() { return shadowWidth; }
	GLsizei GetShadowHeight() { return shadowHeight; }

	~ShadowMap();

protected:
	GLuint FBO; // 帧缓冲对象ID
	GLuint shadowMap; // 阴影贴图ID
	GLsizei shadowWidth, shadowHeight; // 阴影贴图的宽度和高度
};

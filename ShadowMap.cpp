#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;
}

bool ShadowMap::Init(GLsizei width, GLsizei height)
{
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &FBO); // 生成帧缓冲对象


	glGenTextures(1, &shadowMap); // 生成阴影贴图
	glBindTexture(GL_TEXTURE_2D, shadowMap); // 绑定阴影贴图
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
	             nullptr); // 为阴影贴图分配内存

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float bColour[] = {1.0f, 1.0f, 1.0f, 1.0f}; // 设置边缘颜色
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColour); // 设置边缘颜色
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // 绑定帧缓冲对象
	// 将阴影贴图附加到帧缓冲对象的深度缓冲区
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); // 检查帧缓冲对象是否完整

	if (status != GL_FRAMEBUFFER_COMPLETE) // 如果帧缓冲对象不完整
	{
		printf("Framebuffer error: %i\n", status);
		return false;
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // 解绑帧缓冲对象

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // 绑定帧缓冲对象
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit); // 激活纹理单元
	glBindTexture(GL_TEXTURE_2D, shadowMap); // 绑定阴影贴图
}

ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO); // 删除帧缓冲对象
	}

	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap); // 删除阴影贴图
	}
}

#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

bool Texture::LoadTexture()
{
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); // 使用stb_image加载纹理
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); // 生成纹理
	glBindTexture(GL_TEXTURE_2D, textureID); // 绑定纹理

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData); // 生成纹理，使用GL_RGB

	glGenerateMipmap(GL_TEXTURE_2D); // 生成多级渐远纹理

	glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理

	stbi_image_free(texData); // 释放图像内存

	return true;
}

// TODO: perf texture with alpha channel load function still has some problems, like nanosuit model
bool Texture::LoadTextureA()
{
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 4); // 使用stb_image加载纹理
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); // 生成纹理
	glBindTexture(GL_TEXTURE_2D, textureID); // 绑定纹理

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); // 生成纹理，使用GL_RGBA

	glGenerateMipmap(GL_TEXTURE_2D); // 生成多级渐远纹理

	glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理

	stbi_image_free(texData); // 释放图像内存

	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0); // 激活纹理单元（在某些显卡上可以忽略此设置，因为0是默认值）
	glBindTexture(GL_TEXTURE_2D, textureID); // 绑定纹理
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID); // 删除纹理
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture()
{
	ClearTexture();
}

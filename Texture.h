#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture(); // 加载没有Alpha通道的纹理，返回是否加载成功，不成功则加载我们规定的默认纹理
	bool LoadTextureA(); // 加载有Alpha通道的纹理

	void UseTexture(); // 使用纹理
	void ClearTexture(); // 清除纹理

	~Texture();

private:
	GLuint textureID; // 纹理ID
	int width, height; // 纹理宽度和高度
	int bitDepth; // 纹理位深度

	const char* fileLocation; // 纹理文件位置
};


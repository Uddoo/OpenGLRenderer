#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);

	void LoadTexture(); // 加载纹理
	void UseTexture(); // 使用纹理
	void ClearTexture(); // 清除纹理

	~Texture();

private:
	GLuint textureID; // 纹理ID
	int width, height; // 纹理宽度和高度
	int bitDepth; // 纹理位深度

	char* fileLocation; // 纹理文件位置
};


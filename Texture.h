#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	void LoadTexture(); // ��������
	void UseTexture(); // ʹ������
	void ClearTexture(); // �������

	~Texture();

private:
	GLuint textureID; // ����ID
	int width, height; // �����Ⱥ͸߶�
	int bitDepth; // ����λ���

	const char* fileLocation; // �����ļ�λ��
};


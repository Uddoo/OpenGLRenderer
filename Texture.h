#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);

	void LoadTexture(); // ��������
	void UseTexture(); // ʹ������
	void ClearTexture(); // �������

	~Texture();

private:
	GLuint textureID; // ����ID
	int width, height; // �����Ⱥ͸߶�
	int bitDepth; // ����λ���

	char* fileLocation; // �����ļ�λ��
};


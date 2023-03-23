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
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); // ʹ��stb_image��������
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); // ��������
	glBindTexture(GL_TEXTURE_2D, textureID); // ������

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData); // ��������ʹ��GL_RGB

	glGenerateMipmap(GL_TEXTURE_2D); // ���ɶ༶��Զ����

	glBindTexture(GL_TEXTURE_2D, 0); // �������

	stbi_image_free(texData); // �ͷ�ͼ���ڴ�

	return true;
}

// TODO: perf texture with alpha channel load function still has some problems, like nanosuit model
bool Texture::LoadTextureA()
{
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 4); // ʹ��stb_image��������
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); // ��������
	glBindTexture(GL_TEXTURE_2D, textureID); // ������

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); // ��������ʹ��GL_RGBA

	glGenerateMipmap(GL_TEXTURE_2D); // ���ɶ༶��Զ����

	glBindTexture(GL_TEXTURE_2D, 0); // �������

	stbi_image_free(texData); // �ͷ�ͼ���ڴ�

	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0); // ��������Ԫ����ĳЩ�Կ��Ͽ��Ժ��Դ����ã���Ϊ0��Ĭ��ֵ��
	glBindTexture(GL_TEXTURE_2D, textureID); // ������
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID); // ɾ������
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

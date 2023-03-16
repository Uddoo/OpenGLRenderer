#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode); // ���ַ���������ɫ��
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation); // ���ļ�������ɫ��

	std::string ReadFile(const char* fileLocation); // ��ȡ�ļ�

	GLint GetProjectionLocation(); // ��ȡͶӰλ��
	GLint GetModelLocation(); // ��ȡģ��λ��

	void UseShader(); // ʹ����ɫ��
	void ClearShader(); // �����ɫ��

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel; // shaderID = shader ID, uniformProjection = uniform projection, uniformModel = uniform model

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};


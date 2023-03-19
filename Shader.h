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

	GLuint GetProjectionLocation(); // ��ȡͶӰλ��
	GLuint GetModelLocation(); // ��ȡģ��λ��
	GLuint GetViewLocation(); // ��ȡ��ͼλ��

	GLuint GetAmbientIntensityLocation(); // ��ȡ������ǿ��λ��
	GLuint GetAmbientColourLocation(); // ��ȡ��������ɫλ��

	GLuint GetDiffuseIntensityLocation(); // ��ȡ������ǿ��λ��
	GLuint GetDirectionLocation(); // ��ȡ�����䷽��λ��

	void UseShader(); // ʹ����ɫ��
	void ClearShader(); // �����ɫ��

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel; // shaderID = shader ID, uniformProjection = uniform projection, uniformModel = uniform model
	GLuint uniformView; // uniformView = uniform view
	GLuint uniformAmbientIntensity, uniformAmbientColour; // uniformAmbientIntensity = uniform ambient intensity, uniformAmbientColour = uniform ambient colour
	GLuint uniformDiffuseIntensity, uniformDirection; // uniformDiffuseIntensity = uniform diffuse intensity, uniformDirection = uniform direction

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
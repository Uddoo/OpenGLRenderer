#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"

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

	GLuint GetSpecularIntensityLocation(); // ��ȡ�����ǿ��λ��
	GLuint GetShininessLocation(); // ��ȡ�߹�ָ��λ��
	GLuint GetEyePositionLocation(); // ��ȡ�۾�λ��

	void SetDirectionalLight(DirectionalLight* dLight); // ����ƽ�й�

	void SetPointLights(PointLight* pLight, unsigned int lightCount); // ���õ��Դ

	void UseShader(); // ʹ����ɫ��
	void ClearShader(); // �����ɫ��

	~Shader();

private:
	int pointLightCount; // ���Դ����

	GLuint shaderID, uniformProjection, uniformModel; // shaderID = shader ID, uniformProjection = uniform projection, uniformModel = uniform model
	GLuint uniformView; // uniformView = uniform view
	GLuint uniformEyePosition; // uniformEyePosition = uniform eye position

	GLuint uniformSpecularIntensity, uniformShininess; // uniformSpecularIntensity = uniform specular intensity, uniformShininess = uniform shininess

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	}uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	}uniformPointLight[MAX_POINT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
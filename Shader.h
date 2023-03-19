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

	void CreateFromString(const char* vertexCode, const char* fragmentCode); // 从字符串创建着色器
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation); // 从文件创建着色器

	std::string ReadFile(const char* fileLocation); // 读取文件

	GLuint GetProjectionLocation(); // 获取投影位置
	GLuint GetModelLocation(); // 获取模型位置
	GLuint GetViewLocation(); // 获取视图位置

	GLuint GetAmbientIntensityLocation(); // 获取环境光强度位置
	GLuint GetAmbientColourLocation(); // 获取环境光颜色位置

	GLuint GetDiffuseIntensityLocation(); // 获取漫反射强度位置
	GLuint GetDirectionLocation(); // 获取漫反射方向位置

	GLuint GetSpecularIntensityLocation(); // 获取镜面光强度位置
	GLuint GetShininessLocation(); // 获取高光指数位置
	GLuint GetEyePositionLocation(); // 获取眼睛位置

	void SetDirectionalLight(DirectionalLight* dLight); // 设置平行光

	void SetPointLights(PointLight* pLight, unsigned int lightCount); // 设置点光源

	void UseShader(); // 使用着色器
	void ClearShader(); // 清除着色器

	~Shader();

private:
	int pointLightCount; // 点光源数量

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
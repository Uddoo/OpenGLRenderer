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

	void UseShader(); // 使用着色器
	void ClearShader(); // 清除着色器

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel; // shaderID = shader ID, uniformProjection = uniform projection, uniformModel = uniform model
	GLuint uniformView; // uniformView = uniform view
	GLuint uniformAmbientIntensity, uniformAmbientColour; // uniformAmbientIntensity = uniform ambient intensity, uniformAmbientColour = uniform ambient colour
	GLuint uniformDiffuseIntensity, uniformDirection; // uniformDiffuseIntensity = uniform diffuse intensity, uniformDirection = uniform direction

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
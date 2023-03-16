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

	GLint GetProjectionLocation(); // 获取投影位置
	GLint GetModelLocation(); // 获取模型位置

	void UseShader(); // 使用着色器
	void ClearShader(); // 清除着色器

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel; // shaderID = shader ID, uniformProjection = uniform projection, uniformModel = uniform model

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};


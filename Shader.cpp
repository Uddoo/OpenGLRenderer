#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColourLocation()
{
	return uniformAmbientColour;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

void Shader::UseShader()
{
	glUseProgram(shaderID); // 使用着色器
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID); // 删除着色器
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram(); // 创建着色器程序

	if (!shaderID)
	{
		printf("Error creating shader program!");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID); // 链接着色器
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // 检查链接状态
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); // 获取错误信息
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID); // 验证着色器
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result); // 检查验证状态
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); // 获取错误信息
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model"); // 获取 uniform 变量 model 的位置
	uniformProjection = glGetUniformLocation(shaderID, "projection"); // 获取 uniform 变量 projection 的位置
	uniformView = glGetUniformLocation(shaderID, "view"); // 获取 uniform 变量 view 的位置

	uniformAmbientColour = glGetUniformLocation(shaderID, "directionalLight.colour"); // 获取 uniform 变量 directionalLight.colour 的位置
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity"); // 获取 uniform 变量 directionalLight.ambientIntensity 的位置

	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction"); // 获取 uniform 变量 directionalLight.direction 的位置
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity"); // 获取 uniform 变量 directionalLight.diffuseIntensity 的位置
	// 上面这一行原来写成了 == ， 导致没有漫反射光效果

	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity"); // 获取 uniform 变量 material.specularIntensity 的位置
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess"); // 获取 uniform 变量 material.shininess 的位置
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition"); // 获取 uniform 变量 eyePosition 的位置
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType); // 创建着色器

	// 指定着色器源代码
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	// 指定源代码长度
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength); // 把着色器源代码附加到着色器对象上
	glCompileShader(theShader); // 编译着色器

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); // 检查链接状态
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog); // 获取错误信息
		fprintf(stderr, "Error compling the %d shader : '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader); // 把着色器附加到着色器程序上
}
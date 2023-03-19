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
	glUseProgram(shaderID); // ʹ����ɫ��
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID); // ɾ����ɫ��
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
	shaderID = glCreateProgram(); // ������ɫ������

	if (!shaderID)
	{
		printf("Error creating shader program!");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID); // ������ɫ��
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // �������״̬
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); // ��ȡ������Ϣ
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID); // ��֤��ɫ��
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result); // �����֤״̬
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); // ��ȡ������Ϣ
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model"); // ��ȡ uniform ���� model ��λ��
	uniformProjection = glGetUniformLocation(shaderID, "projection"); // ��ȡ uniform ���� projection ��λ��
	uniformView = glGetUniformLocation(shaderID, "view"); // ��ȡ uniform ���� view ��λ��

	uniformAmbientColour = glGetUniformLocation(shaderID, "directionalLight.colour"); // ��ȡ uniform ���� directionalLight.colour ��λ��
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity"); // ��ȡ uniform ���� directionalLight.ambientIntensity ��λ��

	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction"); // ��ȡ uniform ���� directionalLight.direction ��λ��
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity"); // ��ȡ uniform ���� directionalLight.diffuseIntensity ��λ��
	// ������һ��ԭ��д���� == �� ����û���������Ч��

	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity"); // ��ȡ uniform ���� material.specularIntensity ��λ��
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess"); // ��ȡ uniform ���� material.shininess ��λ��
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition"); // ��ȡ uniform ���� eyePosition ��λ��
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType); // ������ɫ��

	// ָ����ɫ��Դ����
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	// ָ��Դ���볤��
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength); // ����ɫ��Դ���븽�ӵ���ɫ��������
	glCompileShader(theShader); // ������ɫ��

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); // �������״̬
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog); // ��ȡ������Ϣ
		fprintf(stderr, "Error compling the %d shader : '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader); // ����ɫ�����ӵ���ɫ��������
}
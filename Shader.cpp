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

GLint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLint Shader::GetModelLocation()
{
	return uniformModel;
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

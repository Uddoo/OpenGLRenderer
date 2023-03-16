#include <iostream>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f; // 角度转弧度

GLuint VAO, VBO, shader, uniformModel; // VAO = Vertex Array Object, VBO = Vertex Buffer Object

bool direction = true;
float triOffset = 0.0f; // 三角形偏移量
float triMaxOffset = 0.7f; // 三角形最大偏移量
float triIncrement = 0.0055f; // 三角形偏移量增量

float curAngle = 0.0f; // 当前角度

bool sizeDirection = true; // 大小方向
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "													\n\
#version 330																	\n\
																				\n\
layout (location = 0) in vec3 pos;												\n\
																				\n\
out vec4 vCol;																	\n\
																				\n\
uniform mat4 model;																\n\
																				\n\
void main()																		\n\
{																				\n\
	gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);						\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);									\n\
}";

// Fragment Shader
static const char* fShader = "													\n\
#version 330																	\n\
																				\n\
in vec4 vCol;																	\n\
																				\n\
out vec4 colour;																\n\
																				\n\
void main()																		\n\
{																				\n\
colour = vCol;																	\n\
}";

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO); // 生成一个 VAO
	glBindVertexArray(VAO); // 绑定 VAO

	glGenBuffers(1, &VBO); // 生成一个 VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定 VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把顶点数组复制到缓冲中供 OpenGL 使用

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // 设置顶点属性指针
	glEnableVertexAttribArray(0); // 启用顶点属性

	glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑 VBO

	glBindVertexArray(0); // 解绑 VAO
}

// 添加着色器
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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

// 编译着色器
void CompileShaders()
{
	shader = glCreateProgram(); // 创建着色器程序

	if (!shader)
	{
		printf("Error creating shader program!");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader); // 链接着色器
	glGetProgramiv(shader, GL_LINK_STATUS, &result); // 检查链接状态
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); // 获取错误信息
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader); // 验证着色器
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result); // 检查验证状态
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); // 获取错误信息
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model"); // 获取 uniform 变量的位置
}

int main()
{
	std::cout << "Hello OpenGL!\n";

	// 初始化 GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed");
		glfwTerminate();
		return 1;
	}

	// 设置 GLFW 窗口属性
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window creation faild");
		glfwTerminate();
		return 1;
	}

	// 获取缓冲大小信息
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // 获取缓冲大小
	// 这里原来写成了 glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferWidth); 导致无法显示三角形


	// 设置上下文
	glfwMakeContextCurrent(mainWindow);

	// 允许使用 GLEW 实验功能
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation faild");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// 创建并设置设置视口大小
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle(); // 创建三角形
	CompileShaders(); // 编译着色器

	// 循环，直到窗口关闭
	while (!glfwWindowShouldClose(mainWindow))
	{
		// 获取并处理事件
		glfwPollEvents();

		// 三角形移动
		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.05f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.01f;
		}
		else
		{
			curSize -= 0.01f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// 清除窗口
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader); // 使用着色器

		glm::mat4 model(1.0f); // 创建模型矩阵

		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f)); // 平移
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // 旋转
		model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 (矩阵)

		glBindVertexArray(VAO); // 绑定 VAO
		glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
		glBindVertexArray(0); // 解绑 VAO

		glUseProgram(0); // 不使用着色器

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
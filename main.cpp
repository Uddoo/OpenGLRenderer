#include <iostream>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f; // 角度转弧度

std::vector<Mesh*> meshList; // 网格列表

std::vector<Shader*> shaderList; // 着色器列表

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
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	// 顶点索引
	unsigned int indices[] = {
			0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 1, 2
	};

	// 顶点数组
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
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

	glEnable(GL_DEPTH_TEST); // 开启深度测试

	// 创建并设置设置视口大小
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateObjects(); // 创建三角形
	CreateShaders(); // 创建着色器

	GLuint uniformProjection = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f); // 创建投影矩阵

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲和深度缓冲

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		glm::mat4 model(1.0f); // 创建模型矩阵

		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 1.0f)); // 旋转
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.0f)); // 平移
		model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); // 设置 uniform 变量 projection
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.0f)); // 平移
		model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		meshList[1]->RenderMesh();

		glUseProgram(0); // 不使用着色器

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
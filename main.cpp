#include <iostream>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

const float toRadians = 3.14159265f / 180.0f; // 角度转弧度

Window mainWindow; // 主窗口

Camera camera; // 相机

GLfloat deltaTime = 0.0f; // 帧间隔时间
GLfloat lastTime = 0.0f;

std::vector<Mesh*> meshList; // 网格列表

std::vector<Shader*> shaderList; // 着色器列表

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
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects(); // 创建三角形
	CreateShaders(); // 创建着色器

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 100.0f); // 创建投影矩阵

	// 循环，直到窗口关闭
	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime) * 1000 / SDL_GetPerformanceFrequency();
		lastTime = now;

		// 获取并处理事件
		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime); // 键盘控制
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange()); // 鼠标控制

		// 清除窗口
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲和深度缓冲

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();

		glm::mat4 model(1.0f); // 创建模型矩阵

		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 1.0f)); // 旋转
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); // 平移
		model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); // 设置 uniform 变量 projection
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix())); // 设置 uniform 变量 view
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.0f)); // 平移
		model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		meshList[1]->RenderMesh();

		glUseProgram(0); // 不使用着色器

		mainWindow.SwapBuffers();
	}

	return 0;
}
// OpenGLCourseApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// 
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
	std::cout << "Hello World!\n";

	// 初始化 GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation faild");
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
	int bufferWidth = 0, bufferHeight = 0;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferWidth);


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

	// 设置视口大小
	glViewport(0, 0, bufferWidth, bufferHeight);

	// 循环，直到窗口关闭
	while (!glfwWindowShouldClose(mainWindow))
	{
		// 获取并处理事件
		glfwPollEvents();

		// 清除窗口
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::Initialise()
{
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

	mainWindow = glfwCreateWindow(width, height, "OpenGLRenderer", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window creation faild");
		glfwTerminate();
		return 1;
	}

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
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

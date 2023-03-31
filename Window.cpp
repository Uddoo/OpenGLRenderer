#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
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

	mainWindow = glfwCreateWindow(width, height, "OpenGLRenderer", nullptr, nullptr);

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

	// 处理键盘和鼠标输入
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标

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

	glfwSetWindowUserPointer(mainWindow, this); // 设置窗口用户指针
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys); // 设置键盘按键回调函数
	glfwSetCursorPosCallback(mainWindow, handleMouse); // 设置鼠标移动回调函数
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	auto theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // 获取窗口用户指针

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // 按下 ESC 键
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 设置窗口应该关闭
	}

	if (key >= 0 && key < 1024) // 如果键入的key是有效值
	{
		if (action == GLFW_PRESS) // 按下
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) // 释放
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	auto theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // 获取窗口用户指针

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; // 避免上下颠倒

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

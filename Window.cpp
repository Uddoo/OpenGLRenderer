#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	// ��ʼ�� GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed");
		glfwTerminate();
		return 1;
	}

	// ���� GLFW ��������
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

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // ��ȡ�����С
	// ����ԭ��д���� glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferWidth); �����޷���ʾ������


	// ����������
	glfwMakeContextCurrent(mainWindow);

	// ������̺��������
	CreateCallbacks();

	// ����ʹ�� GLEW ʵ�鹦��
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation faild");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); // ������Ȳ���

	// ���������������ӿڴ�С
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this); // ���ô����û�ָ��
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys); // ���ü��̰����ص�����
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // ��ȡ�����û�ָ��

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // ���� ESC ��
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // ���ô���Ӧ�ùر�
	}

	if (key >= 0 && key < 1024) // ��������key����Чֵ
	{
		if (action == GLFW_PRESS) // ����
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) // �ͷ�
		{
			theWindow->keys[key] = false;
		}
	}
}

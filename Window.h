#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise(); // ��ʼ��

	GLfloat GetBufferWidth() const { return bufferWidth; }
	GLfloat GetBufferHeight() const { return bufferHeight; }

	bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); } // ��ȡ�����Ƿ�Ӧ�ùر�

	bool* GetKeys() { return keys; } // ��ȡ���̰�������

	GLfloat GetXChange();
	GLfloat GetYChange();

	void SwapBuffers() const { glfwSwapBuffers(mainWindow); } // ����������

	~Window();

private:
	GLFWwindow* mainWindow; // ������

	GLint width, height; // ���ڿ�Ⱥ͸߶�
	GLint bufferWidth, bufferHeight; // ��������Ⱥ͸߶�

	bool keys[1024]; // ���̰���

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange = 0.0f;
	GLfloat yChange = 0.0f;

	bool mouseFirstMoved; // ����Ƿ��һ���ƶ�

	void CreateCallbacks(); // �����ص�����
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode); // ���̰����ص�����
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos); // ����ƶ��ص�����
};


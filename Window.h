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

	void SwapBuffers() const { glfwSwapBuffers(mainWindow); } // ����������

	~Window();

private:
	GLFWwindow* mainWindow; // ������

	GLint width, height; // ���ڿ�Ⱥ͸߶�
	GLint bufferWidth, bufferHeight; // ��������Ⱥ͸߶�

	bool keys[1024]; // ���̰���

	void CreateCallbacks(); // �����ص�����
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode); // ���̰����ص�����
};


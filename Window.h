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
};


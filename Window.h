#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise(); // 初始化

	GLfloat GetBufferWidth() const { return bufferWidth; }
	GLfloat GetBufferHeight() const { return bufferHeight; }

	bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); } // 获取窗口是否应该关闭

	void SwapBuffers() const { glfwSwapBuffers(mainWindow); } // 交换缓冲区

	~Window();

private:
	GLFWwindow* mainWindow; // 主窗口

	GLint width, height; // 窗口宽度和高度
	GLint bufferWidth, bufferHeight; // 缓冲区宽度和高度

	bool keys[1024]; // 键盘按键

	void CreateCallbacks(); // 创建回调函数
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode); // 键盘按键回调函数
};


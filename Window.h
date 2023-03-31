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

	GLint getBufferWidth() const { return bufferWidth; }
	GLint getBufferHeight() const { return bufferHeight; }

	bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); } // 获取窗口是否应该关闭

	bool* getsKeys() { return keys; } // 获取键盘按键数组

	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() const { glfwSwapBuffers(mainWindow); } // 交换缓冲区

	~Window();

private:
	GLFWwindow* mainWindow; // 主窗口

	GLint width, height; // 窗口宽度和高度
	GLint bufferWidth, bufferHeight; // 缓冲区宽度和高度

	bool keys[1024]; // 键盘按键

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange = 0.0f;
	GLfloat yChange = 0.0f;

	bool mouseFirstMoved; // 鼠标是否第一次移动

	void createCallbacks(); // 创建回调函数
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); // 键盘按键回调函数
	static void handleMouse(GLFWwindow* window, double xPos, double yPos); // 鼠标移动回调函数
};


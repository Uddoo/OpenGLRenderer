#pragma once

#include <GL\glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();

	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 GetCameraPosition() { return position; }
	glm::vec3 GetCameraDirection() { return glm::normalize(front); }

	glm::mat4 CalculateViewMatrix(); // 计算观察矩阵

	~Camera();

private:
	glm::vec3 position; // 相机位置
	glm::vec3 front; // 相机前向量
	glm::vec3 up; // 相机上向量
	glm::vec3 right; // 相机右向量
	glm::vec3 worldUp; // 世界上向量

	GLfloat yaw; // 偏航角（左右转动）
	GLfloat pitch; // 俯仰角（上下转动）

	GLfloat moveSpeed; // 移动速度
	GLfloat turnSpeed; // 转动速度

	void Update();
};


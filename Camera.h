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

	glm::mat4 CalculateViewMatrix(); // ����۲����

	~Camera();

private:
	glm::vec3 position; // ���λ��
	glm::vec3 front; // ���ǰ����
	glm::vec3 up; // ���������
	glm::vec3 right; // ���������
	glm::vec3 worldUp; // ����������

	GLfloat yaw; // ƫ���ǣ�����ת����
	GLfloat pitch; // �����ǣ�����ת����

	GLfloat moveSpeed; // �ƶ��ٶ�
	GLfloat turnSpeed; // ת���ٶ�

	void Update();
};

